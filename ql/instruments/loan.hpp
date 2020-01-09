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

/*! \file loan.hpp
  \brief concrete loan class
*/

#ifndef quantlib_loan_hpp
#define quantlib_loan_hpp

#include <ql/instrument.hpp>

#include <ql/time/calendar.hpp>
#include <ql/cashflow.hpp>
#include <ql/compounding.hpp>

#include <vector>

namespace QuantLib
{

    class DayCounter;

    //! Base loan class
    /*! Derived classes must fill the uninitialized data members.

      \warning Most methods assume that the cash flows are stored
      sorted by date, the principal(s) being after any
      cash flow at the same date. In particular, if there's
      one single principal, it must be the last cash flow,

      \ingroup instruments

      \test
      - price/yield calculations are cross-checked for consistency.
      - price/yield calculations are checked against known good
      values.
    */
    class Loan : public Instrument
    {
    public:
        //! constructor for amortizing loan.
        /*! Principals and maturity are calculated from the coupon
          data, if available.  Therefore, principals must not be
          included in the passed cash flows.
        //*/
        Loan(Natural settlementDays,
             const Calendar &calendar,
             const Date &issueDate = Date(),
             const Leg& coupons = Leg());

        class arguments;
        class results;
        class engine;

        //! \name Instrument interface
        //@{
        bool isExpired() const;
        //@}
        //! \name Inspectors
        //@{
        Natural settlementDays() const;
        const Calendar &calendar() const;

        const std::vector<Real> &notionals() const;
        virtual Real notional(Date d = Date()) const;

        /*! \note returns all the cashflows, including the redemptions. */
        const Leg &cashflows() const;
        /*! returns just the redemption flows (not interest payments) */
        const Leg &redemptions() const;
        /*! returns the redemption, if only one is defined */
        const boost::shared_ptr<CashFlow>& redemption() const;

        Date startDate() const;
        Date maturityDate() const;
        Date issueDate() const;

        bool isTradable(Date d = Date()) const;
        Date settlementDate(const Date &d = Date()) const;
        //@}

        //! \name Calculations
        //@{

        //! theoretical clean price
        /*! The default loan settlement is used for calculation.

          \warning the theoretical price calculated from a flat term
          structure might differ slightly from the price
          calculated from the corresponding yield by means
          of the other overload of this function. If the
          price from a constant yield is desired, it is
          advisable to use such other overload.
        */
        Real cleanPrice() const;

        //! theoretical dirty price
        /*! The default loan settlement is used for calculation.

          \warning the theoretical price calculated from a flat term
          structure might differ slightly from the price
          calculated from the corresponding yield by means
          of the other overload of this function. If the
          price from a constant yield is desired, it is
          advisable to use such other overload.
        */
        Real dirtyPrice() const;

        //! theoretical settlement value
        /*! The default bond settlement date is used for calculation. */
        Real settlementValue() const;

        //! theoretical bond yield
        /*! The default bond settlement and theoretical price are used
          for calculation.
        */
        Rate yield(const DayCounter &dc,
                   Compounding comp,
                   Frequency freq,
                   Real accuracy = 1.0e-8,
                   Size maxEvaluations = 100) const;

        //! clean price given a yield and settlement date
        /*! The default bond settlement is used if no date is given. */
        Real cleanPrice(Rate yield,
                        const DayCounter &dc,
                        Compounding comp,
                        Frequency freq,
                        Date settlementDate = Date()) const;

        //! dirty price given a yield and settlement date
        /*! The default bond settlement is used if no date is given. */
        Real dirtyPrice(Rate yield,
                        const DayCounter &dc,
                        Compounding comp,
                        Frequency freq,
                        Date settlementDate = Date()) const;

        //! settlement value as a function of the clean price
        /*! The default bond settlement date is used for calculation. */
        Real settlementValue(Real cleanPrice) const;
 

        //! yield given a (clean) price and settlement date
        /*! The default loan settlement is used if no date is given. */
        Rate yield(Real cleanPrice,
                   const DayCounter &dc,
                   Compounding comp,
                   Frequency freq,
                   Date settlementDate = Date(),
                   Real accuracy = 1.0e-8,
                   Size maxEvaluations = 100) const;

        //! accrued amount at a given date
        /*! The default loan settlement is used if no date is given. */
        virtual Real accruedAmount(Date d = Date()) const;
        //@}

        /*! Expected next coupon: depending on (the loan and) the given date
          the coupon can be historic, deterministic or expected in a
          stochastic sense. When the loan settlement date is used the coupon
          is the already-fixed not-yet-paid one.

          The current loan settlement is used if no date is given.
        */
        virtual Rate nextCouponRate(Date d = Date()) const;

        //! Previous coupon already paid at a given date
        /*! Expected previous coupon: depending on (the loan and) the given
          date the coupon can be historic, deterministic or expected in a
          stochastic sense. When the loan settlement date is used the coupon
          is the last paid one.

          The current loan settlement is used if no date is given.
        */
        Rate previousCouponRate(Date d = Date()) const;

        Date nextCashFlowDate(Date d = Date()) const;
        Date previousCashFlowDate(Date d = Date()) const;
        //@}

    protected:
        void setupExpired() const;
        void setupArguments(PricingEngine::arguments *) const;
        void fetchResults(const PricingEngine::results *) const;

        /*! This method can be called by derived classes in order to
          build principal payments from the existing cash flows.
          It must be called after setting up the cashflows_ vector
          and will fill the notionalSchedule_, notionals_, and
          principals_ data members.

          If given, the elements of the principals vector will
          multiply the amount of the principal cash flow.  The
          elements will be taken in base 100, i.e., a principal
          equal to 100 does not modify the amount.

          \pre The cashflows_ vector must contain at least one
          coupon and must be sorted by date.
        */
        void addRedemptionsToCashflows(const std::vector<Real>& redemptions
                                       = std::vector<Real>());

        /*! This method can be called by derived classes in order to
          build a bond with a single redemption payment.  It will
          fill the notionalSchedule_, notionals_, and redemptions_
          data members.
        */

        void setSingleRedemption(Real notional,
                                 Real redemption,
                                 const Date& date);

        //void calculateServicingFees(const std::vector<std::vector<Rate> >& servicingFees,
        //							  const std::vector<std::vector<DayCounter> >& servicingDayCounters);

        /*! This method can be called by derived classes in order to
          build a loan with a single principal payment.  It will
          fill the notionalSchedule_, notionals_, and principals_
          data members.
        */

        void setSingleRedemption(Real notional,
                                 const boost::shared_ptr<CashFlow> &redemption);

        /*! used internally to collect notional information from the
          coupons. It should not be called by derived classes,
          unless they already provide redemption cash flows (in
          which case they must set up the redemptions_ data member
          independently).  It will fill the notionalSchedule_ and
          notionals_ data members.
        */

        void calculateNotionalsFromCashflows();
 
        Natural settlementDays_;
        Calendar calendar_;
        std::vector<Date> notionalSchedule_;
        std::vector<Real> notionals_;  
        Leg cashflows_;
        Leg redemptions_;

        Date maturityDate_, issueDate_;
        mutable Real settlementValue_;
    };

    class Loan::arguments : public PricingEngine::arguments
    {
    public:
        Date settlementDate;
        Leg cashflows;
        Calendar calendar;
        void validate() const;
    };

    class Loan::results : public Instrument::results
    {
    public:
        Real settlementValue;
        void reset()
        {
            settlementValue = Null<Real>();
            Instrument::results::reset();
        }
    };

    class Loan::engine : public GenericEngine<Loan::arguments,
                                              Loan::results>
    {
    };

    // inline definitions

    inline Natural Loan::settlementDays() const
    {
        return settlementDays_;
    }

    inline const Calendar &Loan::calendar() const
    {
        return calendar_;
    }

    inline const std::vector<Real> &Loan::notionals() const
    {
        return notionals_;
    }

    inline const Leg &Loan::cashflows() const
    {
        return cashflows_;
    }

    inline const Leg &Loan::redemptions() const
    {
        return redemptions_;
    }

    inline Date Loan::issueDate() const
    {
        return issueDate_;
    }
}

#endif

