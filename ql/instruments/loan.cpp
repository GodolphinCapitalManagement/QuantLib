/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
  Copyright (C) 2017 Gyan Sinha

  This file is part of QuantLib, a free-software/open-source library
  for financial quantitative analysts and developers - http://quantlib.org/

  QuantLib is free software: you can redistribute it and/or modify it
  under the terms of the QuantLib license.  You should have received a
  copy of the license along with this program; if not, please email
  <quantlib-dev@lists.sf.net>. The license is also available online at
  <http://quantlib.org/license.shtml>.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/instruments/loan.hpp>
#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/coupon.hpp>
#include <ql/math/solvers1d/brent.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <ql/pricingengines/loan/discountingloanengine.hpp>
#include <ql/pricingengines/loan/loanfunctions.hpp>

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

namespace QuantLib
{

    Loan::Loan(Natural settlementDays,
               const Calendar &calendar,
               const Date &issueDate,
               const Leg &coupons)
        : settlementDays_(settlementDays), calendar_(calendar),
          cashflows_(coupons), issueDate_(issueDate)
    {

        if (!coupons.empty())
            {
                std::sort(cashflows_.begin(), cashflows_.end(),
                          earlier_than<boost::shared_ptr<CashFlow> >());

                if (issueDate_ != Date())
                    {
                        QL_REQUIRE(issueDate_ < cashflows_[0]->date(),
                                   "issue date (" << issueDate_ << ") must be earlier than first payment date (" << cashflows_[0]->date() << ")");
                    }

                maturityDate_ = coupons.back()->date();

                addRedemptionsToCashflows();
            }

        registerWith(Settings::instance().evaluationDate());
    }

    bool Loan::isExpired() const
    {
        // this is the Instrument interface, so it doesn't use
        // LoanFunctions, and includeSettlementDateFlows is true
        // (unless QL_TODAY_PAYMENTS will set it to false later on)
        return CashFlows::isExpired(cashflows_,
                                    true,
                                    Settings::instance().evaluationDate());
    }

    Real Loan::notional(Date d) const
    {
        if (d == Date())
            d = settlementDate();

        if (d > notionalSchedule_.back())
            {
                // after maturity
                return 0.0;
            }

        // After the check above, d is between the schedule
        // boundaries.  We search starting from the second notional
        // date, since the first is null.  After the call to
        // lower_bound, *i is the earliest date which is greater or
        // equal than d.  Its index is greater or equal to 1.
        std::vector<Date>::const_iterator i =
            std::lower_bound(++notionalSchedule_.begin(),
                             notionalSchedule_.end(), d);
        Size index = std::distance(notionalSchedule_.begin(), i);

        if (d < notionalSchedule_[index])
            {
                // no doubt about what to return
                return notionals_[index - 1];
            }
        else
            {
                return notionals_[index];
            }
    }

    const shared_ptr<CashFlow> &Loan::redemption() const
    {
        QL_REQUIRE(redemptions_.size() == 1,
                   "multiple redemption cash flows given");
        return redemptions_.back();
    }

    Date Loan::startDate() const
    {
        return LoanFunctions::startDate(*this);
    }

    Date Loan::maturityDate() const
    {
        if (maturityDate_ != Null<Date>())
            return maturityDate_;
        else
            return LoanFunctions::maturityDate(*this);
    }

    bool Loan::isTradable(Date d) const
    {
        return LoanFunctions::isTradable(*this, d);
    }

    Date Loan::settlementDate(const Date &date) const
    {
        Date d = (date == Date() ? Settings::instance().evaluationDate() : date);

        // usually, the settlement is at T+n...
        Date settlement = calendar_.advance(d, settlementDays_, Days);
        // ...but the loan won't be traded until the issue date (if given.)
        if (issueDate_ == Date())
            return settlement;
        else
            return std::max(settlement, issueDate_);
    }

    Real Loan::cleanPrice() const
    {
        return dirtyPrice() - accruedAmount(settlementDate());
    }

    Real Loan::dirtyPrice() const
    {
        return settlementValue() / notional(settlementDate()) * 100.0;
    }

    Real Loan::settlementValue() const
    {
        calculate();
        QL_REQUIRE(settlementValue_ != Null<Real>(),
                   "settlement value not provided");
        return settlementValue_;
    }

    Real Loan::settlementValue(Real cleanPrice) const
    {
        Real dirtyPrice = cleanPrice + accruedAmount(settlementDate());
        return dirtyPrice / 100.0 * notional(settlementDate());
    }

    Rate Loan::yield(const DayCounter &dc,
                     Compounding comp,
                     Frequency freq,
                     Real accuracy,
                     Size maxEvaluations) const
    {
        Real currentNotional = notional(settlementDate());
        if (currentNotional == 0.0)
            return 0.0;

        return LoanFunctions::yield(*this, cleanPrice(), dc, comp, freq,
                                    settlementDate(),
                                    accuracy, maxEvaluations);
    }

    Real Loan::cleanPrice(Rate y,
                          const DayCounter &dc,
                          Compounding comp,
                          Frequency freq,
                          Date settlement) const
    {
        return LoanFunctions::cleanPrice(*this, y, dc, comp, freq, settlement);
    }

    Real Loan::dirtyPrice(Rate y,
                          const DayCounter &dc,
                          Compounding comp,
                          Frequency freq,
                          Date settlement) const
    {
        Real currentNotional = notional(settlement);
        if (currentNotional == 0.0)
            return 0.0;

        return LoanFunctions::cleanPrice(*this, y, dc, comp, freq, settlement) + accruedAmount(settlement);
    }

    Rate Loan::yield(Real cleanPrice,
                     const DayCounter &dc,
                     Compounding comp,
                     Frequency freq,
                     Date settlement,
                     Real accuracy,
                     Size maxEvaluations) const
    {
        Real currentNotional = notional(settlement);
        if (currentNotional == 0.0)
            return 0.0;

        return LoanFunctions::yield(*this, cleanPrice, dc, comp, freq,
                                    settlement, accuracy, maxEvaluations);
    }

    Real Loan::accruedAmount(Date settlement) const
    {
        Real currentNotional = notional(settlement);
        if (currentNotional == 0.0)
            return 0.0;

        return LoanFunctions::accruedAmount(*this, settlement);
    }

    Rate Loan::nextCouponRate(Date settlement) const
    {
        return LoanFunctions::nextCouponRate(*this, settlement);
    }

    Rate Loan::previousCouponRate(Date settlement) const
    {
        return LoanFunctions::previousCouponRate(*this, settlement);
    }

    Date Loan::nextCashFlowDate(Date settlement) const
    {
        return LoanFunctions::nextCashFlowDate(*this, settlement);
    }

    Date Loan::previousCashFlowDate(Date settlement) const
    {
        return LoanFunctions::previousCashFlowDate(*this, settlement);
    }

    void Loan::setupExpired() const
    {
        Instrument::setupExpired();
        settlementValue_ = 0.0;
    }

    void Loan::setupArguments(PricingEngine::arguments *args) const
    {
        Loan::arguments *arguments = dynamic_cast<Loan::arguments *>(args);
        QL_REQUIRE(arguments != 0, "wrong argument type");

        arguments->settlementDate = settlementDate();
        arguments->cashflows = cashflows_;
        arguments->calendar = calendar_;
    }

    void Loan::fetchResults(const PricingEngine::results *r) const
    {

        Instrument::fetchResults(r);

        const Loan::results *results =
            dynamic_cast<const Loan::results *>(r);
        QL_ENSURE(results != 0, "wrong result type");

        settlementValue_ = results->settlementValue;
    }

    void Loan::addRedemptionsToCashflows(const std::vector<Real> &redemptions)
    {
        // First, we gather the notional information from the cashflows
        calculateNotionalsFromCashflows();
        // Then, we create the redemptions based on the notional
        // information and we add them to the cashflows vector after
        // the coupons.
        redemptions_.clear();
        for (Size i = 1; i < notionalSchedule_.size(); ++i)
            {
                Real R = i < redemptions.size() ? redemptions[i] : !redemptions.empty() ? redemptions.back() : 100.0;
                Real amount = (R / 100.0) * (notionals_[i - 1] - notionals_[i]);
                shared_ptr<CashFlow> payment;
                if (i < notionalSchedule_.size() - 1)
                    payment.reset(new AmortizingPayment(amount,
                                                        notionalSchedule_[i]));
                else
                    payment.reset(new Redemption(amount, notionalSchedule_[i]));
                cashflows_.push_back(payment);
                redemptions_.push_back(payment);
            }
        // stable_sort now moves the redemptions to the right places
        // while ensuring that they follow coupons with the same date.
        std::stable_sort(cashflows_.begin(), cashflows_.end(),
                         earlier_than<shared_ptr<CashFlow> >());
    }

    void Loan::setSingleRedemption(Real notional,
                                   Real redemption,
                                   const Date &date)
    {

        shared_ptr<CashFlow> redemptionCashflow(
                                                new Redemption(notional * redemption / 100.0, date));
        setSingleRedemption(notional, redemptionCashflow);
    }

    void Loan::setSingleRedemption(Real notional,
                                   const shared_ptr<CashFlow> &redemption)
    {
        notionals_.resize(2);
        notionalSchedule_.resize(2);
        redemptions_.clear();

        notionalSchedule_[0] = Date();
        notionals_[0] = notional;

        notionalSchedule_[1] = redemption->date();
        notionals_[1] = 0.0;

        cashflows_.push_back(redemption);
        redemptions_.push_back(redemption);
    }

    void Loan::calculateNotionalsFromCashflows()
    {
        notionalSchedule_.clear();
        notionals_.clear();

        Date lastPaymentDate = Date();
        notionalSchedule_.push_back(Date());
        for (Size i = 0; i < cashflows_.size(); ++i)
            {
                shared_ptr<Coupon> coupon =
                    boost::dynamic_pointer_cast<Coupon>(cashflows_[i]);
                if (!coupon)
                    continue;

                Real notional = coupon->nominal();
                // we add the notional only if it is the first one...
                if (notionals_.empty())
                    {
                        notionals_.push_back(coupon->nominal());
                        lastPaymentDate = coupon->date();
                    }
                else if (!close(notional, notionals_.back()))
                    {
                        // ...or if it has changed.
                        QL_REQUIRE(notional < notionals_.back(),
                                   "increasing coupon notionals");
                        notionals_.push_back(coupon->nominal());
                        // in this case, we also add the last valid date for
                        // the previous one...
                        notionalSchedule_.push_back(lastPaymentDate);
                        // ...and store the candidate for this one.
                        lastPaymentDate = coupon->date();
                    }
                else
                    {
                        // otherwise, we just extend the valid range of dates
                        // for the current notional.
                        lastPaymentDate = coupon->date();
                    }
            }
        QL_REQUIRE(!notionals_.empty(), "no coupons provided");
        notionals_.push_back(0.0);
        notionalSchedule_.push_back(lastPaymentDate);
    }

    void Loan::arguments::validate() const
    {
        QL_REQUIRE(settlementDate != Date(), "no settlement date provided");
        QL_REQUIRE(!cashflows.empty(), "no cashflows provided");
        for (Size i = 0; i < cashflows.size(); ++i)
            QL_REQUIRE(cashflows[i], "null cash flow provided");
    }
}

