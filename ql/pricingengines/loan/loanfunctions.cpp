/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
  Copyright (C) 2017 Gyan Sinha
  Copyright (C) 2009 Nathan Abbott
  Copyright (C) 2007, 2008, 2009, 2010 Ferdinando Ametrano
  Copyright (C) 2007 Chiara Fornarola
  Copyright (C) 2008 Simon Ibbotson
  Copyright (C) 2004 M-Dimension Consulting Inc.
  Copyright (C) 2005, 2006, 2007, 2008, 2009 StatPro Italia srl
  Copyright (C) 2004 Jeff Yu

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

#include <ql/pricingengines/loan/loanfunctions.hpp>
#include <ql/instruments/loan.hpp>
#include <ql/cashflows/cashflows.hpp>

using boost::shared_ptr;

namespace QuantLib {

    Date LoanFunctions::startDate(const Loan& loan) {
        return CashFlows::startDate(loan.cashflows());
    }

    Date LoanFunctions::maturityDate(const Loan& loan) {
        return CashFlows::maturityDate(loan.cashflows());
    }

    bool LoanFunctions::isTradable(const Loan& loan,
                                   Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return loan.notional(settlement)!=0.0;
    }

    Leg::const_reverse_iterator
    LoanFunctions::previousCashFlow(const Loan& loan,
                                    Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::previousCashFlow(loan.cashflows(),
                                           false, settlement);
    }

    Leg::const_iterator LoanFunctions::nextCashFlow(const Loan& loan,
                                                    Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::nextCashFlow(loan.cashflows(),
                                       false, settlement);
    }

    Date LoanFunctions::previousCashFlowDate(const Loan& loan,
                                             Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::previousCashFlowDate(loan.cashflows(),
                                               false, settlement);
    }

    Date LoanFunctions::nextCashFlowDate(const Loan& loan,
                                         Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::nextCashFlowDate(loan.cashflows(),
                                           false, settlement);
    }

    Real LoanFunctions::previousCashFlowAmount(const Loan& loan,
                                               Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::previousCashFlowAmount(loan.cashflows(),
                                                 false, settlement);
    }

    Real LoanFunctions::nextCashFlowAmount(const Loan& loan,
                                           Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::nextCashFlowAmount(loan.cashflows(),
                                             false, settlement);
    }

    Rate LoanFunctions::previousCouponRate(const Loan& loan,
                                           Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::previousCouponRate(loan.cashflows(),
                                             false, settlement);
    }

    Rate LoanFunctions::nextCouponRate(const Loan& loan,
                                       Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        return CashFlows::nextCouponRate(loan.cashflows(),
                                         false, settlement);
    }

    Date LoanFunctions::accrualStartDate(const Loan& loan,
                                         Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accrualStartDate(loan.cashflows(),
                                           false, settlement);
    }

    Date LoanFunctions::accrualEndDate(const Loan& loan,
                                       Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accrualEndDate(loan.cashflows(),
                                         false, settlement);
    }

    Date LoanFunctions::referencePeriodStart(const Loan& loan,
                                             Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::referencePeriodStart(loan.cashflows(),
                                               false, settlement);
    }

    Date LoanFunctions::referencePeriodEnd(const Loan& loan,
                                           Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::referencePeriodEnd(loan.cashflows(),
                                             false, settlement);
    }

    Time LoanFunctions::accrualPeriod(const Loan& loan,
                                      Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accrualPeriod(loan.cashflows(),
                                        false, settlement);
    }

    Date::serial_type LoanFunctions::accrualDays(const Loan& loan,
                                                 Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accrualDays(loan.cashflows(),
                                      false, settlement);
    }

    Time LoanFunctions::accruedPeriod(const Loan& loan,
                                      Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accruedPeriod(loan.cashflows(),
                                        false, settlement);
    }

    Date::serial_type LoanFunctions::accruedDays(const Loan& loan,
                                                 Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accruedDays(loan.cashflows(),
                                      false, settlement);
    }

    Real LoanFunctions::accruedAmount(const Loan& loan,
                                      Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::accruedAmount(loan.cashflows(),
                                        false, settlement) *
            100.0 / loan.notional(settlement);
    }



    Real LoanFunctions::cleanPrice(const Loan& loan,
                                   const YieldTermStructure& discountCurve,
                                   Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " settlement date (maturity being " <<
                   loan.maturityDate() << ")");

        Real dirtyPrice = CashFlows::npv(loan.cashflows(), discountCurve,
                                         false, settlement) *
            100.0 / loan.notional(settlement);
        return dirtyPrice - loan.accruedAmount(settlement);
    }

    Real LoanFunctions::bps(const Loan& loan,
                            const YieldTermStructure& discountCurve,
                            Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::bps(loan.cashflows(), discountCurve,
                              false, settlement) *
            100.0 / loan.notional(settlement);
    }

    Rate LoanFunctions::atmRate(const Loan& loan,
                                const YieldTermStructure& discountCurve,
                                Date settlement,
                                Real cleanPrice) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        Real dirtyPrice = cleanPrice==Null<Real>() ? Null<Real>() :
            cleanPrice + loan.accruedAmount(settlement);
        Real currentNotional = loan.notional(settlement);
        Real npv = dirtyPrice==Null<Real>() ? Null<Real>() :
            dirtyPrice/100.0 * currentNotional;

        return CashFlows::atmRate(loan.cashflows(), discountCurve,
                                  false, settlement, settlement,
                                  npv);
    }

    Real LoanFunctions::cleanPrice(const Loan& loan,
                                   const InterestRate& yield,
                                   Date settlement) {
        return dirtyPrice(loan, yield, settlement) - loan.accruedAmount(settlement);
    }

    Real LoanFunctions::cleanPrice(const Loan& loan,
                                   Rate yield,
                                   const DayCounter& dayCounter,
                                   Compounding compounding,
                                   Frequency frequency,
                                   Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return cleanPrice(loan, y, settlement);
    }

    Real LoanFunctions::dirtyPrice(const Loan& loan,
                                   const InterestRate& yield,
                                   Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        Real dirtyPrice = CashFlows::npv(loan.cashflows(), yield,
                                         false, settlement) *
            100.0 / loan.notional(settlement);
        return dirtyPrice;
    }

    Real LoanFunctions::dirtyPrice(const Loan& loan,
                                   Rate yield,
                                   const DayCounter& dayCounter,
                                   Compounding compounding,
                                   Frequency frequency,
                                   Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return dirtyPrice(loan, y, settlement);
    }

    Real LoanFunctions::bps(const Loan& loan,
                            const InterestRate& yield,
                            Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::bps(loan.cashflows(), yield,
                              false, settlement) *
            100.0 / loan.notional(settlement);
    }

    Real LoanFunctions::bps(const Loan& loan,
                            Rate yield,
                            const DayCounter& dayCounter,
                            Compounding compounding,
                            Frequency frequency,
                            Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return bps(loan, y, settlement);
    }

    Rate LoanFunctions::yield(const Loan& loan,
                              Real cleanPrice,
                              const DayCounter& dayCounter,
                              Compounding compounding,
                              Frequency frequency,
                              Date settlement,
                              Real accuracy,
                              Size maxIterations,
                              Rate guess) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        Real dirtyPrice = cleanPrice + loan.accruedAmount(settlement);
        dirtyPrice /= 100.0 / loan.notional(settlement);

        return CashFlows::yield(loan.cashflows(), dirtyPrice,
                                dayCounter, compounding, frequency,
                                false, settlement, settlement,
                                accuracy, maxIterations, guess);
    }

    Time LoanFunctions::duration(const Loan& loan,
                                 const InterestRate& yield,
                                 Duration::Type type,
                                 Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::duration(loan.cashflows(), yield,
                                   type,
                                   false, settlement);
    }

    Time LoanFunctions::duration(const Loan& loan,
                                 Rate yield,
                                 const DayCounter& dayCounter,
                                 Compounding compounding,
                                 Frequency frequency,
                                 Duration::Type type,
                                 Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return duration(loan, y, type, settlement);
    }

    Real LoanFunctions::convexity(const Loan& loan,
                                  const InterestRate& yield,
                                  Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::convexity(loan.cashflows(), yield,
                                    false, settlement);
    }

    Real LoanFunctions::convexity(const Loan& loan,
                                  Rate yield,
                                  const DayCounter& dayCounter,
                                  Compounding compounding,
                                  Frequency frequency,
                                  Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return convexity(loan, y, settlement);
    }

    Real LoanFunctions::basisPointValue(const Loan& loan,
                                        const InterestRate& yield,
                                        Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::basisPointValue(loan.cashflows(), yield,
                                          false, settlement);
    }

    Real LoanFunctions::basisPointValue(const Loan& loan,
                                        Rate yield,
                                        const DayCounter& dayCounter,
                                        Compounding compounding,
                                        Frequency frequency,
                                        Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return CashFlows::basisPointValue(loan.cashflows(), y,
                                          false, settlement);
    }

    Real LoanFunctions::yieldValueBasisPoint(const Loan& loan,
                                             const InterestRate& yield,
                                             Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        return CashFlows::yieldValueBasisPoint(loan.cashflows(), yield,
                                               false, settlement);
    }

    Real LoanFunctions::yieldValueBasisPoint(const Loan& loan,
                                             Rate yield,
                                             const DayCounter& dayCounter,
                                             Compounding compounding,
                                             Frequency frequency,
                                             Date settlement) {
        InterestRate y(yield, dayCounter, compounding, frequency);
        return CashFlows::yieldValueBasisPoint(loan.cashflows(), y,
                                               false, settlement);
    }

    Real LoanFunctions::cleanPrice(const Loan& loan,
                                   const shared_ptr<YieldTermStructure>& d,
                                   Spread zSpread,
                                   const DayCounter& dc,
                                   Compounding comp,
                                   Frequency freq,
                                   Date settlement) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        Real dirtyPrice = CashFlows::npv(loan.cashflows(), d,
                                         zSpread, dc, comp, freq,
                                         false, settlement) *
            100.0 / loan.notional(settlement);
        return dirtyPrice - loan.accruedAmount(settlement);
    }

    Spread LoanFunctions::zSpread(const Loan& loan,
                                  Real cleanPrice,
                                  const shared_ptr<YieldTermStructure>& d,
                                  const DayCounter& dayCounter,
                                  Compounding compounding,
                                  Frequency frequency,
                                  Date settlement,
                                  Real accuracy,
                                  Size maxIterations,
                                  Rate guess) {
        if (settlement == Date())
            settlement = loan.settlementDate();

        QL_REQUIRE(LoanFunctions::isTradable(loan, settlement),
                   "non tradable at " << settlement <<
                   " (maturity being " << loan.maturityDate() << ")");

        Real dirtyPrice = cleanPrice + loan.accruedAmount(settlement);
        dirtyPrice /= 100.0 / loan.notional(settlement);

        return CashFlows::zSpread(loan.cashflows(),
                                  d,
                                  dirtyPrice,
                                  dayCounter, compounding, frequency,
                                  false, settlement, settlement,
                                  accuracy, maxIterations, guess);
    }

}
