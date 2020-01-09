/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
  Copyright (C) 2007, 2008, 2009, 2010 Ferdinando Ametrano
  Copyright (C) 2007 Chiara Fornarola
  Copyright (C) 2009 StatPro Italia srl
  Copyright (C) 2009 Nathan Abbott
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

/*! \file loanfunctions.hpp
  \brief loan functions
*/

#ifndef quantlib_loan_functions_hpp
#define quantlib_loan_functions_hpp

#include <ql/cashflows/duration.hpp>
#include <ql/cashflow.hpp>
#include <ql/interestrate.hpp>
#include <boost/shared_ptr.hpp>

namespace QuantLib {

    // forward declarations
    class Loan;
    class DayCounter;
    class YieldTermStructure;

    //! Loan adapters of CashFlows functions
    /*! See CashFlows for functions' documentation.

      These adapters calls into CashFlows functions passing as input the
      Loan cashflows, the dirty price (i.e. npv) calculated from clean
      price, the loan settlement date (unless another date is given), zero
      ex-dividend days, and excluding any cashflow on the settlement date.

      Prices are always clean, as per market convention.
    */
    struct LoanFunctions {
        //! \name Date inspectors
        //@{
        static Date startDate(const Loan& loan);
        static Date maturityDate(const Loan& loan);
        static bool isTradable(const Loan& loan,
                               Date settlementDate = Date());
        //@}

        //! \name CashFlow inspectors
        //@{
        static Leg::const_reverse_iterator
        previousCashFlow(const Loan& loan,
                         Date refDate = Date());
        static Leg::const_iterator nextCashFlow(const Loan& loan,
                                                Date refDate = Date());
        static Date previousCashFlowDate(const Loan& loan,
                                         Date refDate = Date());
        static Date nextCashFlowDate(const Loan& loan,
                                     Date refDate = Date());
        static Real previousCashFlowAmount(const Loan& loan,
                                           Date refDate = Date());
        static Real nextCashFlowAmount(const Loan& loan,
                                       Date refDate = Date());
        //@}

        //! \name Coupon inspectors
        //@{
        static Rate previousCouponRate(const Loan& loan,
                                       Date settlementDate = Date());
        static Rate nextCouponRate(const Loan& loan,
                                   Date settlementDate = Date());
        static Date accrualStartDate(const Loan& loan,
                                     Date settlementDate = Date());
        static Date accrualEndDate(const Loan& loan,
                                   Date settlementDate = Date());
        static Date referencePeriodStart(const Loan& loan,
                                         Date settlementDate = Date());
        static Date referencePeriodEnd(const Loan& loan,
                                       Date settlementDate = Date());
        static Time accrualPeriod(const Loan& loan,
                                  Date settlementDate = Date());
        static Date::serial_type accrualDays(const Loan& loan,
                                             Date settlementDate = Date());
        static Time accruedPeriod(const Loan& loan,
                                  Date settlementDate = Date());
        static Date::serial_type accruedDays(const Loan& loan,
                                             Date settlementDate = Date());
        static Real accruedAmount(const Loan& loan,
                                  Date settlementDate = Date());
        //@}

        //! \name YieldTermStructure functions
        //@{
        static Real cleanPrice(const Loan& loan,
                               const YieldTermStructure& discountCurve,
                               Date settlementDate = Date());
        static Real bps(const Loan& loan,
                        const YieldTermStructure& discountCurve,
                        Date settlementDate = Date());
        static Rate atmRate(const Loan& loan,
                            const YieldTermStructure& discountCurve,
                            Date settlementDate = Date(),
                            Real cleanPrice = Null<Real>());
        //@}

        //! \name Yield (a.k.a. Internal Rate of Return, i.e. IRR) functions
        //@{
        static Real cleanPrice(const Loan& loan,
                               const InterestRate& yield,
                               Date settlementDate = Date());
        static Real cleanPrice(const Loan& loan,
                               Rate yield,
                               const DayCounter& dayCounter,
                               Compounding compounding,
                               Frequency frequency,
                               Date settlementDate = Date());
        static Real dirtyPrice(const Loan& loan,
                               const InterestRate& yield,
                               Date settlementDate = Date());
        static Real dirtyPrice(const Loan& loan,
                               Rate yield,
                               const DayCounter& dayCounter,
                               Compounding compounding,
                               Frequency frequency,
                               Date settlementDate = Date());
        static Real bps(const Loan& loan,
                        const InterestRate& yield,
                        Date settlementDate = Date());
        static Real bps(const Loan& loan,
                        Rate yield,
                        const DayCounter& dayCounter,
                        Compounding compounding,
                        Frequency frequency,
                        Date settlementDate = Date());
        static Rate yield(const Loan& loan,
                          Real cleanPrice,
                          const DayCounter& dayCounter,
                          Compounding compounding,
                          Frequency frequency,
                          Date settlementDate = Date(),
                          Real accuracy = 1.0e-10,
                          Size maxIterations = 100,
                          Rate guess = 0.05);
        static Time duration(const Loan& loan,
                             const InterestRate& yield,
                             Duration::Type type = Duration::Modified,
                             Date settlementDate = Date() );
        static Time duration(const Loan& loan,
                             Rate yield,
                             const DayCounter& dayCounter,
                             Compounding compounding,
                             Frequency frequency,
                             Duration::Type type = Duration::Modified,
                             Date settlementDate = Date() );
        static Real convexity(const Loan& loan,
                              const InterestRate& yield,
                              Date settlementDate = Date());
        static Real convexity(const Loan& loan,
                              Rate yield,
                              const DayCounter& dayCounter,
                              Compounding compounding,
                              Frequency frequency,
                              Date settlementDate = Date());
        static Real basisPointValue(const Loan& loan,
                                    const InterestRate& yield,
                                    Date settlementDate = Date());
        static Real basisPointValue(const Loan& loan,
                                    Rate yield,
                                    const DayCounter& dayCounter,
                                    Compounding compounding,
                                    Frequency frequency,
                                    Date settlementDate = Date());
        static Real yieldValueBasisPoint(const Loan& loan,
                                         const InterestRate& yield,
                                         Date settlementDate = Date());
        static Real yieldValueBasisPoint(const Loan& loan,
                                         Rate yield,
                                         const DayCounter& dayCounter,
                                         Compounding compounding,
                                         Frequency frequency,
                                         Date settlementDate = Date());
        //@}

        //! \name Z-spread functions
        //@{
        static Real cleanPrice(const Loan& loan,
                               const boost::shared_ptr<YieldTermStructure>& discount,
                               Spread zSpread,
                               const DayCounter& dayCounter,
                               Compounding compounding,
                               Frequency frequency,
                               Date settlementDate = Date());
        static Spread zSpread(const Loan& loan,
                              Real cleanPrice,
                              const boost::shared_ptr<YieldTermStructure>&,
                              const DayCounter& dayCounter,
                              Compounding compounding,
                              Frequency frequency,
                              Date settlementDate = Date(),
                              Real accuracy = 1.0e-10,
                              Size maxIterations = 100,
                              Rate guess = 0.0);
        //@}

    };

}

#endif
