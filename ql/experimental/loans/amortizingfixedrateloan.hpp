/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2008 Simon Ibbotson

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

/*! \file amortizingfixedrateloan.hpp
    \brief amortizing fixed-rate loan
*/

#ifndef quantlib_amortizing_fixed_rate_loan_hpp
#define quantlib_amortizing_fixed_rate_loan_hpp

#include <ql/instruments/loan.hpp>
#include <ql/time/daycounter.hpp>

namespace QuantLib {

    class Schedule;

    //! amortizing fixed-rate loan
    class AmortizingFixedRateLoan : public Loan {
      public:
        AmortizingFixedRateLoan(
                          Natural settlementDays,
                          const std::vector<Real>& notionals,
                          const Schedule& schedule,
                          const std::vector<Rate>& coupons,
                          const DayCounter& accrualDayCounter,
                          BusinessDayConvention paymentConvention = Following,
                          const Date& issueDate = Date());
        /*! Automatically generates a set of equal coupons, with an
            amortizing loan.  The coupons are equal and the accrual
            daycount is only used for quoting/settlement purposes -
            not for calculating the coupons.
        */
        AmortizingFixedRateLoan(Natural settlementDays,
                                const Calendar& calendar,
                                Real faceAmount,
                                const Date& startDate,
                                const Period& loanTenor,
                                const Frequency& sinkingFrequency,
                                Rate coupon,
                                const DayCounter& accrualDayCounter,
                                BusinessDayConvention paymentConvention = Following,
                                const Date& issueDate = Date());
        Frequency frequency() const { return frequency_; }
        const DayCounter& dayCounter() const { return dayCounter_; }
      protected:
        Frequency frequency_;
        DayCounter dayCounter_;
    };

}

#endif

