#include "stdafx.h"
#include "PrimeTables.h"

// "a" has at most two factors (a = p1 * p2 or a = p^2 or a is prime)
// We know that p1 >= p and p2 <= q
//
// We have 3 cases:
// 1) a is prime, S(a) = a + 1
// 2) a = p * q, where q = a / p
//		S(a) = (p + 1) * (a / p + 1) = a + p + a / p + 1
//
//		Let's find the maximum possible value for S(a):
//
//		f(x) = a + x + a / x + 1 -> max, 2 < p <= x <= a / p
//		f'(x) = 1 - a / x^2
//		we have an optimum of f(x) when x = sqrt(a)
//		f''(x) = 2 * a / x^3 > 0, so it's a minimum
//		the minimum is reached when x = sqrt(a)
//		the maximum is reached when x = p or x = a / p
//
//		x1 = p:		f(x1) = a + p + a / p + 1
//		x2 = a / p:	f(x2) = a + a / p + p + 1
//
//		So the maximum possible value for S(a) is f(x1) = f(x2) = a + p + a / p + 1
// 3) a = p^2, S(a) = 1 + p + p^2 = 1 + p + a
//
// Value from case (2) is always greater than value from case (3):
// a + p + a / p + 1 - (1 + p + a) = a + p + a / p + 1 - 1 - p - a = a / p > 0
//
// So, S(a) <= a + p + a / p + 1 = a + p + q + 1
FORCEINLINE number MaximumSumOfDivisors2(const number a, const number p, const number q)
{
	return a + p + q + 1;
}

// a = pqr, p0 <= p < q < r, p0^4 > a, S(a) = (p+1)(q+1)(r+1)
// F(p,q) = (p + 1)(q + 1)(a / pq + 1) = (pq + p + q + 1)(a / pq + 1) = a + a / q + a / p + a / pq + pq + p + q + 1
//
// F(p,q) = a + a / p + a / q + a / pq + pq + p + q + 1 -> max
// p >= p0
// p <= q <= sqrt(a) / p
//
// F'p(p,q) = -a / p^2 - a / p^2q + q + 1
// F'q(p,q) = -a / q^2 - a / pq^2 + p + 1
//
// F''p(p,q) = 2a / p^3 + 2a / p^3q > 0
// F''pq(p,q) = a / p^2q^2 + 1 > 0
//
// F''q(p,q) = 2a / q^3 + 2a / pq^3 > 0
// F''qp(p,q) = a / p^2q^2 + 1 > 0
//
// All second partial derivatives are always positive, therefore F(p, q) doesn't have maximum inside the given area (p >= p0, p <= q <= sqrt(a) / p)
//
// 1) p=p0: F(p0,q) = F0(q) = a + a / p0 + a / q + a / p0q + p0q + p0 + q + 1
//		F0(q) = a + a / p0 + a / q + a / p0q + p0q + p0 + q + 1
//		F0(q) = q * (p0 + 1) + (a + a / p0 + p0 + 1) + (a + a / p0) / q
//
//		F0'(q) = p0 + 1 - (a + a / p0) / q^2
//		F0''(q) = 2 * (a + a / p0) / q^3 > 0
//		therefore F0(q) has a maximum when q = p0 or q = sqrt(a) / p0
//
//		F0(p0) = a + a / p0 + a / p0 + a / p0^2 + p0^2 + p0 + p0 + 1
//		F0(p0) = a + 2*a / p0 + a / p0^2 + p0^2 + 2*p0 + 1
//		F0(p0) = a + 2 * (a / p0 + p0) + 1 + p0^2 + a / p0^2
//		since p0^4 > a then p0^2 > a / p0^2
//		a / p0^2 < p0^2
//		F0(p0) < a + 2 * (a / p0 + p0) + 1 + p0^2 + p0^2
//		F0(p0) < a + 2 * (a / p0 + p0) + 1 + 2 * p0^2
//		F0(p0) < a + 2 * (a / p0 + p0 + p0^2) + 1
//		F0(p0) < a + (a / p0 + p0^2 + p0) * 2 + 1
//		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//		F0(sqrt(a) / p0) = a + a / p0 + a / (sqrt(a) / p0) + a / (p0 * sqrt(a) / p0) + p0 * (sqrt(a) / p0) + p0 + sqrt(a) / p0 + 1
//		F0(sqrt(a) / p0) = a + a / p0 + a / sqrt(a) * p0 + a / sqrt(a) + sqrt(a) + p0 + sqrt(a) / p0 + 1
//		F0(sqrt(a) / p0) = a + a / p0 + sqrt(a) * p0 + sqrt(a) + sqrt(a) + p0 + sqrt(a) / p0 + 1
//		F0(sqrt(a) / p0) = a + a / p0 + sqrt(a) * (p0 + 2 + 1 / p0) + p0 + 1
//		since p0^4 > a then p0^2 > sqrt(a)
//		sqrt(a) < p0^2
//		F0(sqrt(a) / p0) < a + a / p0 + p0^2 * (p0 + 2 + 1 / p0) + p0 + 1
//		F0(sqrt(a) / p0) < a + a / p0 + p0^3 + 2 * p0^2 + p0 * 2 + 1
//		F0(sqrt(a) / p0) < a + a / p0 + p0^3 + (p0^2 + p0) * 2 + 1
//		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//		F0(p0) - F0(sqrt(a) / p0) = a / p0 - p0^3
//		since p0^4 > a then p0^3 > a / p0
//		therefore F0(p0) - F0(sqrt(a) / p0) < 0
//		F0(p0) < F0(sqrt(a) / p0)
//
// 2) q=p: F(p,p) = F1(p) = a + a / p + a / p + a / p^2 + p^2 + p + p + 1
//		F1(p) = a + a / p + a / p + a / p^2 + p^2 + p + p + 1
//		F1(p) = a + 2 * a / p + a / p^2 + p^2 + 2 * p + 1
//		F1(p) = p^2 + 2 * p + 1 + a * (1 + 2 / p + 1 / p^2)
//
//		F1'(p) = 2 * p + 2 + a * (-2 / p^2 - 2 / p^3)
//		F1''(p) = 2 + a * (4 / p^3 + 6 / p^4) > 0
//		therefore F1(p) has a maximum when p = p0 or p = sqrt(a) / p0
//
//		F1(p0) = p0^2 + 2 * p0 + 1 + a * (1 + 2 / p0 + 1 / p0^2)
//		F1(p0) = p0^2 + 2 * p0 + 1 + a + 2 * a / p0 + a / p0^2
//		since p0^4 > a then p0^2 > a / p0^2
//		a / p0^2 < p0^2
//		F1(p0) < p0^2 + 2 * p0 + 1 + a + 2 * a / p0 + p0^2
//		F1(p0) < a + 2 * a / p0 + 2 * p0^2 + 2 * p0 + 1
//		F1(p0) < a + (a / p0 + p0^2 + p0) * 2 + 1 = F0(p0)
//
//		F1(sqrt(a) / p0) = (sqrt(a) / p0)^2 + 2 * (sqrt(a) / p0) + 1 + a * (1 + 2 / (sqrt(a) / p0) + 1 / (sqrt(a) / p0)^2)
//		F1(sqrt(a) / p0) = a / p0^2 + 2 * (sqrt(a) / p0) + 1 + a * (1 + 2 / sqrt(a) * p0 + 1 / (a / p0^2))
//		F1(sqrt(a) / p0) = a / p0^2 + 2 * p0 * sqrt(a) + 1 + a * (1 + 2 * p0 / sqrt(a) + p0^2 / a)
//		F1(sqrt(a) / p0) = a / p0^2 + 2 * p0 * sqrt(a) + 1 + a + 2 * a * p0 / sqrt(a) + p0^2
//		F1(sqrt(a) / p0) = a / p0^2 + 2 * p0 * sqrt(a) + 1 + a + 2 * sqrt(a) * p0 + p0^2
//		F1(sqrt(a) / p0) = a / p0^2 + 4 * p0 * sqrt(a) + 1 + a + p0^2
//		a / p0^2 < p0^2
//		sqrt(a) < p0^2
//		F1(sqrt(a) / p0) < p0^2 + 4 * p0 * p0^2 + 1 + a + p0^2
//		F1(sqrt(a) / p0) < a + 2 * p0^2 + 4 * p0^3 + 1
//		F1(sqrt(a) / p0) < a + 4 * p0^3 + 2 * p0^2 + 1
//		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//		F1(sqrt(a) / p0) - F0(sqrt(a) / p0) = (4 * p0^3) - (a / p0 + p0^3 + p0 * 2)
//		p0^4 > a then p0^3 > a / p0
//		(4 * p0^3) - (a / p0 + p0^3 + p0 * 2) > (4 * p0^3) - (p0^3 + p0^3 + p0 * 2)
//		(4 * p0^3) - (p0^3 * 2 + p0 * 2)
//		(4 * p0^3) - p0^3 * 2 - p0 * 2
//		p0^3 * 2 - p0 * 2
//		2 * p0 * (p0^2 - p0)
//		2 * p0^2 * (p0 - 1) > 0
//
//		F1(sqrt(a) / p0) > F0(sqrt(a) / p0)
//
// 3) q = sqrt(a) / p: F(p, sqrt(a) / p) = F2(p) = a + a / p + a / (sqrt(a) / p) + a / (p * sqrt(a) / p) + p * (sqrt(a) / p) + p + sqrt(a) / p + 1
//		F2(p) = a + a / p + a / (sqrt(a) / p) + a / (p * sqrt(a) / p) + p * (sqrt(a) / p) + p + sqrt(a) / p + 1
//		F2(p) = a + a / p + a / sqrt(a) * p + a / sqrt(a) + sqrt(a) + p + sqrt(a) / p + 1
//		F2(p) = a + a / p + sqrt(a) * p + sqrt(a) * 2 + p + sqrt(a) / p + 1
//		F2(p) = a + a / p + sqrt(a) * (p + 2 + 1 / p) + p + 1
//
//		F2'(p) = -a / p^2 + sqrt(a) * (1 - 1 / p^2) + 1
//		F2''(p) = 2 * a / p^3 + sqrt(a) * 2 / p^3 > 0
//		therefore F2(p) has a maximum when p = p0 or q = p
//		q = p
//		sqrt(a) / p = p
//		sqrt(a) = p^2
//		p^4 = a
//		since p >= p0 and p0^4 > a then we only have to check p = p0:
//
//		F2(p0) = a + a / p0 + sqrt(a) * (p0 + 2 + 1 / p0) + p0 + 1
//		p0^4 > a, p0^2 > sqrt(a), sqrt(a) < p0^2
//		F2(p0) < a + a / p0 + p0^2 * (p0 + 2 + 1 / p0) + p0 + 1
//		F2(p0) < a + a / p0 + p0^3 + 2 * p0^2 + p0 + p0 + 1
//		F2(p0) < a + a / p0 + p0^3 + 2 * p0^2 + 2 * p0 + 1
//		F2(p0) < a + a / p0 + p0^3 + 2 * (p0^2 + p0) + 1
//		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//
//		F2(p0) - F1(sqrt(a) / p0) = a / p0 + 2 * p0 - 3 * p0^3
//		p0^4 > a, p0^3 > a / p0, a / p0 < p0^3
//		a / p0 + 2 * p0 - 3 * p0^3 < p0^3 + 2 * p0 - 3 * p0^3
//		p0^3 + 2 * p0 - 3 * p0^3
//		2 * p0 - 2 * p0^3
//		2 * p0 * (1 - p0^2) < 0
//		which means that F2(p0) < F1(sqrt(a) / p0)
//
//		The result: the largest estimate is
//		F1(sqrt(a) / p0) = F(sqrt(a) / p0, sqrt(a) / p0) = (sqrt(a) / p0 + 1)(sqrt(a) / p0 + 1)(a / (a / p0^2) + 1)
//		(sqrt(a) / p0 + 1)^2 * (a / a * p0^2 + 1)
//		(sqrt(a) / p0 + 1)^2 * (p0^2 + 1)
//		(a / p0^2 + 2 * sqrt(a) / p0 + 1) * (p0^2 + 1)
//		(a / p0^2 + 2 * sqrt(a) / p0 + 1) * p0^2 + (a / p0^2 + 2 * sqrt(a) / p0 + 1)
//		a + 2 * p0 * sqrt(a) + p0^2 + a / p0^2 + 2 * sqrt(a) / p0 + 1
//		sqrt(a) < p0^2
//		a + 2 * p0 * p0^2 + p0^2 + a / p0^2 + 2 * p0^2 / p0 + 1
//		a + 4 * p0^3 + p0^2 + a / p0^2 + 1
//		a / p0^2 < p0^2
//		a + 4 * p0^3 + 2 * p0^2 + 1
FORCEINLINE number MaximumSumOfDivisors3(const number a, const number p0, const number a_div_p0)
{
	// F1(sqrt(a) / p0) is the analytically deduced sup(S(a)) - see comments above
	//
	// I use F0(p0) instead: I analyzed all cases when "a" has 3 factors
	// and concluded that F0(p0) will always be correct - see comments in CheckMaximumSumOfDivisors3()
	// Moreover, comments in CheckMaximumSumOfDivisors3 show that F0(p0) is always strictly greater than S(a)
	// So we can just remove "+ 1".

	//  F0(p0)
	return a + (a_div_p0 + p0 * p0 + p0) * 2 /*+ 1*/;
	
	// F0(sqrt(a) / p0)
	//const number p0_2 = p0 * p0;
	//return a + a_div_p0 + p0_2 * p0 + (p0_2 + p0) * 2 + 1;

	// F1(sqrt(a) / p0)
	//return a + 4 * p0 * p0 * p0 + 2 * p0 * p0 + 1;
}

// Lemma 2.1 from http://www.ams.org/journals/mcom/1986-47-175/S0025-5718-1986-0842142-3/S0025-5718-1986-0842142-3.pdf
FORCEINLINE number GetCoeffForMaximumSumOfDivisorsN(const number m, const number i, number& j)
{
	for (; ; --j)
	{
		const SumEstimateData* data = SumEstimates[j] + i;
		if (m > data->P)
			return data->Q;
	}
}

FORCEINLINE number MaximumSumOfDivisorsN(const number m, const number i, number& j)
{
	number highProduct;
	_umul128(m, GetCoeffForMaximumSumOfDivisorsN(m, i, j), &highProduct);
	return m + highProduct;
}

template<int numPrimesCheckedSoFar>
FORCEINLINE bool IsNumEligible(const number a, const number sumA, const number targetSum, number& j)
{
	number highProduct;
	const number maxPossibleSum = _umul128(sumA, MaximumSumOfDivisorsN(a, numPrimesCheckedSoFar, j), &highProduct);
	return (maxPossibleSum >= targetSum) || highProduct;
}

#define M(X) template<> FORCEINLINE bool IsNumEligible<X>(const number, const number, const number, number&) { return true; }
	M(0)M(1)M(2)M(3)M(4)M(5)M(6)M(7)M(8)M(9)
	M(10)M(11)M(12)M(13)M(14)M(15)
#undef M

template<> FORCEINLINE bool IsNumEligible<IS_NUM_ELIGIBLE_BEGIN>(const number a, const number sumA, const number targetSum, number& j)
{
	for (j = 0; ; ++j)
	{
		const SumEstimateData* data = SumEstimates[j] + IS_NUM_ELIGIBLE_BEGIN;
		if (a <= data->P)
		{
			data = SumEstimates[--j] + IS_NUM_ELIGIBLE_BEGIN;
			number highProduct;
			_umul128(a, data->Q, &highProduct);
			const number maxPossibleSum = _umul128(sumA, a + highProduct, &highProduct);
			return (maxPossibleSum >= targetSum) || highProduct;
		}
	}
}

template<int PrimeIndex>
FORCEINLINE bool CheckDivisibility(number& a, number& sumA, const number targetSum, number& j)
{
	enum {p = CompileTimePrimes<PrimeIndex>::value};

	IF_CONSTEXPR(((PrimeIndex & 7) == 0) && (PrimeIndex != 56))
	{
		if (!IsNumEligible<PrimeIndex>(a, sumA, targetSum, j))
			return false;
	}

	number q = a / p;
	if (q * p == a)
	{
		number n = p;
		number curSum = p + 1;
		a = q;
		for (;;)
		{
			q = a / p;
			if (q * p != a)
				break;
			n *= p;
			curSum += n;
			a = q;
		}
		sumA *= curSum;

		if (a == 1)
			return (sumA == targetSum);

		// found new prime factor, let's check that N is not abundant yet
		if (sumA * (a + 1) > targetSum)
			return false;

		IF_CONSTEXPR((PrimeIndex > IS_NUM_ELIGIBLE_BEGIN * 2) && ((PrimeIndex & 7) != 0) && (((PrimeIndex + 1) & 7) != 0))
		{
			if (!IsNumEligible<PrimeIndex + 1>(a, sumA, targetSum, j))
				return false;
		}

		// and then check that targetSum is divisible by partial sum
		enum Sums : number
		{
			s1 = number(p) + 1,
			s2 = number(p) * p + s1,
			s3 = number(p) * p * p + s2,
			s4 = number(p) * p * p * p + s3,
			s5 = number(p) * p * p * p * p + s4,
		};
		if ((curSum == s1) && ((targetSum / s1) * s1 != targetSum))
			return false;
		IF_CONSTEXPR(PrimeIndex <= 32)
		{
			if ((curSum == s2) && ((targetSum / s2) * s2 != targetSum))
				return false;
		}
		IF_CONSTEXPR(PrimeIndex <= 16)
		{
			if ((curSum == s3) && ((targetSum / s3) * s3 != targetSum))
				return false;
		}
		IF_CONSTEXPR(PrimeIndex <= 8)
		{
			if ((curSum == s4) && ((targetSum / s4) * s4 != targetSum))
				return false;
		}
		IF_CONSTEXPR(PrimeIndex <= 4)
		{
			if ((curSum == s5) && ((targetSum / s5) * s5 != targetSum))
				return false;
		}
	}

	IF_CONSTEXPR(PrimeIndex == 32)
		if ((targetSum % sumA) != 0)
			return false;

	return CheckDivisibility<PrimeIndex + 1>(a, sumA, targetSum, j);
}

template<> FORCEINLINE bool CheckDivisibility<CompileTimePrimesCount>(number&, number&, const number, number&) { return true; }

__declspec(thread) number NumFoundPairs = 0;
number TotalCPUcycles = 0;

NOINLINE void NumberFound(const number n1, const number targetSum)
{
	(void)n1;
	(void)targetSum;
	++NumFoundPairs;
#if !RUN_PERFORMANCE_TEST
	printf("%I64u, %I64u\n", n1, targetSum - n1);
#endif
}

FORCEINLINE number Root4(const number n)
{
	return static_cast<number>(sqrt(sqrt(n)));
}

FORCEINLINE void CheckPairInternal(const number n1, const number targetSum, number n2, number sum)
{
	number indexForMaximumSumOfDivisorsN;
	if (!CheckDivisibility<1>(n2, sum, targetSum, indexForMaximumSumOfDivisorsN))
		return;

	const byte* shift = NextPrimeShifts + CompileTimePrimesCount - 1;
	number p = static_cast<number>(CompileTimePrimes<CompileTimePrimesCount - 1>::value) + *(shift++) * SearchLimit::ShiftMultiplier;
	number p2 = p * p;
 
	if (p2 > n2)
	{
		if (n2 > 1)
			sum *= n2 + 1;

		if (sum == targetSum)
			NumberFound(n1, targetSum);

		return;
	}

	number numPrimesCheckedSoFar = CompileTimePrimesCount;

	if (!IsNumEligible<CompileTimePrimesCount>(n2, sum, targetSum, indexForMaximumSumOfDivisorsN))
		return;

	// We must have sum == targetSum in the end
	// sum is constructed by multiplying it by (p^(e+1) - 1) for each found prime factor p^e
	// p^(e+1) - 1 is an integer number
	//
	// no matter how we split the sum, in "partial_sum1 * partial_sum2 == targetSum" both partial sums must be integer
	//
	// so if targetSum is not divisible by sum, we'll have this: sum * x == targetSum where x is not integer
	// which means we can stop right now
	number n2TargetSum = targetSum / sum;
	if ((targetSum % sum) != 0)
		return;
	number n2_sqrt4 = Root4(n2);

	while (p <= n2_sqrt4)
	{
		number q;
		if (PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
		{
			number n = p;
			number curSum = p + 1;
			n2 = q;

			while (p <= n2)
			{
				if (!PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
					break;
				n *= p;
				curSum += n;
				n2 = q;
			}
			sum *= curSum;

			if (n2 == 1)
			{
				if (sum == targetSum)
					NumberFound(n1, targetSum);
				return;
			}

			// found new prime factor, let's check that N is not abundant yet
			if (sum * (n2 + 1) > targetSum)
				return;

			// and then check that targetSum is still divisible by sum
			n2TargetSum = targetSum / sum;
			if ((targetSum % sum) != 0)
				return;
			n2_sqrt4 = Root4(n2);
		}

		++numPrimesCheckedSoFar;
		if (MaximumSumOfDivisorsN(n2, numPrimesCheckedSoFar, indexForMaximumSumOfDivisorsN) < n2TargetSum)
			return;

		p += *(shift++) * SearchLimit::ShiftMultiplier;
	}

	// Here p^4 > n2, so n2 can have at most 3 factors
	while (p * p * p <= n2)
	{
		number q;
		if (PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
		{
			number n = p;
			number curSum = p + 1;
			n2 = q;

			while (p <= n2)
			{
				if (!PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
					break;
				n *= p;
				curSum += n;
				n2 = q;
			}
			sum *= curSum;

			if (n2 == 1)
			{
				if (sum == targetSum)
					NumberFound(n1, targetSum);
				return;
			}

			// found new prime factor, let's check that N is not abundant yet
			if (sum * (n2 + 1) > targetSum)
				return;

			// and then check that targetSum is still divisible by sum
			n2TargetSum = targetSum / sum;
			if ((targetSum % sum) != 0)
				return;
		}

		if (MaximumSumOfDivisors3(n2, p, q) < n2TargetSum)
			return;

		p += *(shift++) * SearchLimit::ShiftMultiplier;
		++numPrimesCheckedSoFar;
	}

	// Here p^3 > n2, so n2 can have at most 2 factors
	bool firstIteration = true;
	while (p * p <= n2)
	{
		number q;
		if (numPrimesCheckedSoFar < ReciprocalsTableSize)
		{
			if (PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
			{
				number n = p;
				number curSum = p + 1;
				n2 = q;
				while (p <= n2)
				{
					if (!PrimeReciprocals[numPrimesCheckedSoFar].Divide(n2, p, q))
						break;
					n *= p;
					curSum += n;
					n2 = q;
				}
				sum *= curSum;
				break;
			}
			++numPrimesCheckedSoFar;
		}
		else
		{
			q = n2 / p;
			number r = n2 % p;
			if (r == 0)
			{
				number n = p;
				number curSum = p + 1;
				n2 = q;

				while (p <= n2)
				{
					q = n2 / p;
					r = n2 % p;
					if (r != 0)
						break;
					n *= p;
					curSum += n;
					n2 = q;
				}
				sum *= curSum;
				break;
			}
		}

		if (MaximumSumOfDivisors2(n2, p, q) < n2TargetSum)
			return;

		if (firstIteration)
		{
			// If n2 + 1 != n2TargetSum then n2 must be composite to satisfy S(N) = target sum
			if (n2 + 1 != n2TargetSum)
			{
				// If N is a perfect square, then N mod 64 must be one of the following: 0, 1, 4, 9, 16, 17, 25, 33, 36, 41, 49, 57
				enum Modulo64SquareCheck : number
				{
					value =
						(number(1) << 0) |
						(number(1) << 1) |
						(number(1) << 4) |
						(number(1) << 9) |
						(number(1) << 16) |
						(number(1) << 17) |
						(number(1) << 25) |
						(number(1) << 33) |
						(number(1) << 36) |
						(number(1) << 41) |
						(number(1) << 49) |
						(number(1) << 57)
				};

				// 1) Check the case n2 = p*q, p < q
				// A=n2
				// B=n2TargetSum-n2-1
				// D=B^2-4*A
				// p=(B-sqrt(D))/2
				// q=(B+sqrt(D))/2
				const number B = n2TargetSum - n2 - 1;
				const number D = B * B - n2 * 4;
				// 1 << (D % 64) is equivalent to 1 << D on x86-64
				if ((number(1) << D) & Modulo64SquareCheck::value)
				{
					// Using floating point arithmetic gives 52 bits of precision
					// 1 or 2 bits will be lost because of rounding errors
					// but even if 4 bits are lost, this will be correct if sqrt_D < 2^48
					//
					// p and q here must be > cuberoot(N).
					// q - p = sqrt_D, so if q > 2^48 then this code might work incorrectly.
					//
					// Let's try to get a lower bound for such number.
					// N > p * q > p * 2^48 > cuberoot(N) * 2^48
					// N / cuberoot(N) > 2^48
					// N^(2/3) > 2^48
					// N > 2^(48*3/2) = 2^72
					// N must be > 2^72 (which is a very conservative estimate) to break this code
					// Since N is always < 2^64 here, it'll be safe
					//
					// sqrt is rounded up here, so for example sqrt(15241383936) will give 123456.00000000000001
					// static_cast<number>(...) will round it down to a correct integer
					const double D1 = static_cast<double>(B) * B - n2 * 4.0;
					if (D1 > 0)
					{
						const number sqrt_D = static_cast<number>(sqrt(D1));
						if (sqrt_D * sqrt_D == D)
						{
							p = (B - sqrt_D) / 2;
							q = (B + sqrt_D) / 2;
							if (p * q == n2)
								NumberFound(n1, targetSum);
							return;
						}
					}
				}

				// 2) Check the case n2 = p^2
				if ((number(1) << n2) & Modulo64SquareCheck::value)
				{
					p = static_cast<number>(sqrt(static_cast<double>(n2)));
					const number p_squared = p * p;
					if (p_squared == n2)
						if (p_squared + p + 1 == n2TargetSum)
							NumberFound(n1, targetSum);
				}

				// There are no other cases, so just exit now
				return;
			}

			// n2 + 1 == n2TargetSum, n2 must be a prime number
			if (n2 <= SearchLimit::PrimesUpToSqrtLimitValue)
			{
				if (IsPrime(n2))
					NumberFound(n1, targetSum);
				return;
			}

			// We have to continue the loop to verify that n2 is a prime number
			firstIteration = false;
		}

		p += *(shift++) * SearchLimit::ShiftMultiplier;
	}

	if (n2 > 1)
		sum *= n2 + 1;

	if (sum == targetSum)
		NumberFound(n1, targetSum);
}

NOINLINE void CheckPairInternalNoInline(const number n1, const number targetSum, number n2, number sum)
{
	CheckPairInternal(n1, targetSum, n2, sum);
}

FORCEINLINE bool InitialCheck(const number n1, const number targetSum, number& n2, number& sum)
{
	if (n1 <= SearchLimit::value / 10)
		return false;

	// n1 must be a smaller member of a pair
	n2 = targetSum - n1;
	if (n1 >= n2)
		return false;

	sum = 1;

	unsigned long bitIndex;
	_BitScanForward64(&bitIndex, n2);
	if (bitIndex > 0)
	{
		n2 >>= bitIndex;
		const number minSum = n2 + 1;
		if ((minSum << (bitIndex + 1)) - minSum > targetSum)
			return false;
		sum = (number(1) << (bitIndex + 1)) - 1;
	}

	return true;
}

FORCEINLINE void CheckPair(const number n1, const number targetSum)
{
	number n2, sum;
	if (InitialCheck(n1, targetSum, n2, sum))
		CheckPairInternalNoInline(n1, targetSum, n2, sum);
}

template<number Index>
FORCEINLINE bool IsValidCandidate(const number D, const number sumD, number gcd, number sumGCD)
{
	enum { p = CompileTimePrimes<Index>::value };
	number qa = D / p;
	if (qa * p == D)
	{
		number qb = sumD / p;
		if (qb * p == sumD)
		{
			number n = p;
			number curSum = p + 1;
			number a = qa;
			number b = qb;
			for (;;)
			{
				qa = a / p;
				if (qa * p != a)
					break;
				qb = b / p;
				if (qb * p != b)
					break;
				n *= p;
				curSum += n;
				a = qa;
				b = qb;
			}
			gcd *= n;
			sumGCD *= curSum;
		}
	}
	return IsValidCandidate<Index + 1>(D, sumD, gcd, sumGCD);
}

template<> FORCEINLINE bool IsValidCandidate<InlinePrimesInSearch + 1>(const number D, const number sumD, number gcd, number sumGCD)
{
	// "sumGCD * (sumD - D) < gcd * sumD" must be true
	number sum1[2];
	sum1[0] = _umul128(sumGCD, sumD - D, &sum1[1]);

	number sum2[2];
	sum2[0] = _umul128(gcd, sumD, &sum2[1]);

	return ((sum1[1] < sum2[1]) || ((sum1[1] == sum2[1]) && (sum1[0] < sum2[0])));
}

FORCEINLINE bool IsValidCandidate(const number D, const number sumD)
{
	// If one of numbers is odd, then it's very unlikely for it to not be a candidate
	// Such numbers exist, but they're very rare, so it's better to skip factorization and exit
	// My tests show that this check speeds up the search
	const number allBitsCombined = D | sumD;
	if (allBitsCombined & 1)
		return true;

	unsigned long index;
	_BitScanForward64(&index, allBitsCombined);

	const number gcd = number(1) << index;
	const number sumGCD = gcd * 2 - 1;
	return IsValidCandidate<1>(D, sumD, gcd, sumGCD);
}

template<int NumDistinctOddPrimeFactors>
FORCEINLINE void Search(const number aValue, const number aSumOfDivisors, const number aPreviousPrimeFactor)
{
	IF_CONSTEXPR(NumDistinctOddPrimeFactors <= 3)
	{
		if ((aSumOfDivisors / 2 <= aValue) && (GetMaxSumMDiv2(aValue, aSumOfDivisors, aPreviousPrimeFactor) < aValue))
			return;
	}

	IF_CONSTEXPR(NumDistinctOddPrimeFactors == 1)
	{
		if (!IsValidCandidate(aValue, aSumOfDivisors))
			return;
	}

	CheckPair(aValue, aSumOfDivisors);

	number curPrime = CompileTimePrimes<InlinePrimesInSearch + 1>::value;

	number N = aPreviousPrimeFactor;

	// We already have 1 or more distinct prime factors here
	// so curPrime must be < sqrt(SearchLimit::value) here
	// because it must be less than other prime factors
	// therefore we can use the fast way to iterate over primes (shift array)
	IF_CONSTEXPR(NumDistinctOddPrimeFactors == 1)
		if (N > SearchLimit::SQRT2)
			N = SearchLimit::SQRT2;

	const byte* shift = NextPrimeShifts + InlinePrimesInSearch + 1;
	while (curPrime < N)
	{
		number highProductNextValue;
		number nextValue = _umul128(aValue, curPrime, &highProductNextValue);
		if ((nextValue > SearchLimit::value) || highProductNextValue)
			break;

		number n = aSumOfDivisors * curPrime;
		number sumN = aSumOfDivisors + n;
		for (;;)
		{
			Search<NumDistinctOddPrimeFactors + 1>(nextValue, sumN, curPrime);

			nextValue = _umul128(nextValue, curPrime, &highProductNextValue);
			if ((nextValue > SearchLimit::value) || highProductNextValue)
				break;

			n *= curPrime;
			sumN += n;
		}

		curPrime += *(shift++) * SearchLimit::ShiftMultiplier;
	}
}

template<> FORCEINLINE void Search<MaxSearchDepth<SearchLimit::value>::value>(const number aValue, const number aSumOfDivisors, const number)
{
	CheckPair(aValue, aSumOfDivisors);
}

template<int NumDistinctOddPrimeFactors>
NOINLINE void SearchNoInline(const number aValue, const number aSumOfDivisors, const number aPreviousPrimeFactor)
{
	for (const std::pair<number, number>& n : g_SmallFactorNumbersData.myNumbers)
	{
		number h;
		const number D = _umul128(n.first, aValue, &h);
		if (h || (D > SearchLimit::value))
			break;

		const number sumD = n.second * aSumOfDivisors;
		Search<NumDistinctOddPrimeFactors>(D, sumD, aPreviousPrimeFactor);
	}
}

static volatile number SharedCounterForSearch[2] = {};
static number NumThreads = 0;

NOINLINE number GetCacheSizePerThread()
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	DWORD returnLength = 0;
	if (!GetLogicalProcessorInformation(buffer, &returnLength) && (GetLastError() == ERROR_INSUFFICIENT_BUFFER))
	{
		buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
		if (buffer)
		{
			if (GetLogicalProcessorInformation(buffer, &returnLength))
			{
				PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = buffer;
				DWORD byteOffset = 0;
				DWORD L2TotalSize = 0;
				DWORD L3TotalSize = 0;
				while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
				{
					if (ptr->Relationship == RelationCache)
					{
						if (ptr->Cache.Level == 2)
							L2TotalSize += ptr->Cache.Size;
						else if (ptr->Cache.Level == 3)
							L3TotalSize += ptr->Cache.Size;
					}
					byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
					++ptr;
				}
				if (L3TotalSize)
				{
					free(buffer);
					return (L3TotalSize - L3TotalSize / 10) / NumThreads;
				}
				else if (L2TotalSize)
				{
					free(buffer);
					return (L2TotalSize - L2TotalSize / 10) / NumThreads;
				}
			}
			free(buffer);
		}
	}
	return 1024 * 1024;
}

template<>
NOINLINE void Search<0>(const number aRangeStart0, const number aRangeEnd0, const number)
{
	number aRangeStart = aRangeStart0;
	number aRangeEnd = aRangeEnd0;

	if (aRangeEnd > SearchLimit::SafeLimit)
		aRangeEnd = SearchLimit::SafeLimit;

	if (aRangeStart > aRangeEnd)
		aRangeStart = aRangeEnd;

	volatile number* sharedCounterForSearch = SharedCounterForSearch;

	// First iterate up to PrimesUpToSqrtLimitSortedCount using the fast way (precomputed table of primes)
	number curPrime;
	if (aRangeStart < PrimesUpToSqrtLimitSortedCount)
	{
		const number limit = min(aRangeEnd, PrimesUpToSqrtLimitSortedCount - 1) - aRangeStart;
		for (;;)
		{
			const number sharedCounterValue = _InterlockedIncrement(&sharedCounterForSearch[0]) - 1;
			if (sharedCounterValue > limit)
				break;

			const number p = PrimesUpToSqrtLimitSorted[sharedCounterValue + aRangeStart];
			if (p >= SearchLimit::LinearLimit)
			{
				for (auto p2 : LinearSearchData)
				{
					const number n1 = p * p2.first;
					if (n1 > SearchLimit::value)
						break;
					CheckPair(n1, (p + 1) * p2.second);
				}
			}
			else
			{
				number n = p;
				number sumN = 1 + p;
				do
				{
					SearchNoInline<1>(n, sumN, p);
					number highProduct;
					n = _umul128(n, p, &highProduct);
					if (highProduct)
						break;
					sumN += n;
				} while (n <= SearchLimit::value);
			}
		}
		curPrime = SearchLimit::PrimesUpToSqrtLimitValue + 1;
	}
	else
	{
		curPrime = aRangeStart;
	}

	// Then iterate up to aRangeEnd using the slow way (Sieve of Eratosthenes to get next primes)
	const number CacheSizePerThread = GetCacheSizePerThread();
	std::vector<byte> primes;
	primes.reserve(CacheSizePerThread + (NumOffsets / Byte::Bits) * 16);
	const number minRangeSize = (1024 / (NumOffsets / Byte::Bits)) * PrimeTableParameters::Modulo;

	number maxRangeSize = (CacheSizePerThread / (NumOffsets / Byte::Bits)) * PrimeTableParameters::Modulo;
	if (maxRangeSize > (aRangeEnd + 1 - curPrime) / (NumThreads * 4) + 1)
		maxRangeSize = (aRangeEnd + 1 - curPrime) / (NumThreads * 4) + 1;
	if (maxRangeSize < minRangeSize)
		maxRangeSize = minRangeSize;

	number curRangeEnd = curPrime - 1;
	number localCounter = 0;

	do
	{
		const number sharedCounterValue = _InterlockedIncrement(&sharedCounterForSearch[1]);

		number curRangeBegin;
		do
		{
			curRangeBegin = curRangeEnd + 1;
			if (curRangeBegin > aRangeEnd)
				return;

			++localCounter;

			number rangeSize = curRangeBegin / 16;
			if (rangeSize < minRangeSize)
				rangeSize = minRangeSize;
			if (rangeSize > maxRangeSize)
				rangeSize = maxRangeSize;

			curRangeEnd = curRangeBegin + rangeSize;
			if (curRangeEnd > aRangeEnd)
				curRangeEnd = aRangeEnd;
		} while (localCounter < sharedCounterValue);

		const number lowerBound = CalculatePrimes(curRangeBegin, curRangeEnd, primes);
		const number* sieveData = (const number*)(primes.data());
		number moduloIndex = lowerBound;
		number bitIndexShift = 0;
		number curSieveChunk = *(sieveData++);
		const unsigned int* PossiblePrimesForModuloPtr = NumbersCoprimeToModulo;

		do
		{
			while (!curSieveChunk)
			{
				curSieveChunk = *(sieveData++);

				const number NewValues =	(PrimeTableParameters::Modulo / 2)	| (number(PrimeTableParameters::Modulo / 2) << 16)	| (number(PrimeTableParameters::Modulo) << 32) |
											(16 << 8)							| (32 << 24)										| (number(0) << 40);

				moduloIndex += ((NewValues >> bitIndexShift) & 255) * 2;
				bitIndexShift = (NewValues >> (bitIndexShift + 8)) & 255;

				PossiblePrimesForModuloPtr = NumbersCoprimeToModulo + bitIndexShift;
			}

			unsigned long bitIndex;
			_BitScanForward64(&bitIndex, curSieveChunk);
			curSieveChunk &= (curSieveChunk - 1);

			curPrime = moduloIndex + PossiblePrimesForModuloPtr[bitIndex];
		} while (curPrime < curRangeBegin);

		while (curPrime <= curRangeEnd)
		{
			if (curPrime >= SearchLimit::LinearLimit)
			{
				for (auto p : LinearSearchData)
				{
					const number n1 = curPrime * p.first;
					if (n1 > SearchLimit::value)
						break;
					const number targetSum = (curPrime + 1) * p.second;
					number n2, sum;
					if (InitialCheck(n1, targetSum, n2, sum))
						CheckPairInternal(n1, targetSum, n2, sum);
				}
			}
			else
			{
				number n = curPrime;
				number sumN = 1 + curPrime;
				do
				{
					SearchNoInline<1>(n, sumN, curPrime);
					number highProduct;
					n = _umul128(n, curPrime, &highProduct);
					if (highProduct)
						break;
					sumN += n;
				} while (n <= SearchLimit::value);
			}

			while (!curSieveChunk)
			{
				curSieveChunk = *(sieveData++);

				const number NewValues =	(PrimeTableParameters::Modulo / 2)	| (number(PrimeTableParameters::Modulo / 2) << 16)	| (number(PrimeTableParameters::Modulo) << 32) |
											(16 << 8)							| (32 << 24)										| (number(0) << 40);

				moduloIndex += ((NewValues >> bitIndexShift) & 255) * 2;
				bitIndexShift = (NewValues >> (bitIndexShift + 8)) & 255;

				PossiblePrimesForModuloPtr = NumbersCoprimeToModulo + bitIndexShift;
			}

			unsigned long bitIndex;
			_BitScanForward64(&bitIndex, curSieveChunk);
			curSieveChunk &= (curSieveChunk - 1);

			curPrime = moduloIndex + PossiblePrimesForModuloPtr[bitIndex];
		}
	} while (curRangeEnd < aRangeEnd);
}

unsigned int __stdcall SearchThreadFunc(void* aData)
{
	_control87(_RC_UP,  _MCW_RC);
	const number threadIndex = number(aData) & 0xFFFFFFFFUL;
	if (threadIndex == 0)
		for (const std::pair<number, number>& n : g_SmallFactorNumbersData.myNumbers)
			CheckPair(n.first, n.second);
	Search<0>(0, SearchLimit::SafeLimit, 0);
	return static_cast<unsigned int>(NumFoundPairs);
}

#include <process.h>

number RunSearch(number numThreadsOverride)
{
	TotalCPUcycles = 0;
	memset((void*)SharedCounterForSearch, 0, sizeof(SharedCounterForSearch));

	DWORD_PTR processAffinity;
	DWORD_PTR systemAffinity;
	GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity);
	NumThreads = 0;
	for (DWORD_PTR k = processAffinity; k; k &= (k - 1))
		++NumThreads;

	if (numThreadsOverride > 0)
		NumThreads = numThreadsOverride;

	HANDLE handles[64];
	DWORD_PTR threadAffinity = processAffinity;
	for (number i = 0; i < NumThreads; ++i)
	{
		const number threadData = i + (NumThreads << 32);
		handles[i] = (HANDLE) _beginthreadex(0, 0, SearchThreadFunc, (void*)(threadData), CREATE_SUSPENDED, 0);
		unsigned long index;
		_BitScanForward64(&index, threadAffinity);
		if (numThreadsOverride == 0)
			SetThreadAffinityMask(handles[i], number(1) << index);
		threadAffinity &= ~(number(1) << index);
	}

	for (number i = 0; i < NumThreads; ++i)
		ResumeThread(handles[i]);

	WaitForMultipleObjects(static_cast<DWORD>(NumThreads), handles, true, INFINITE);

	number exitCodeSum = 0;
	for (number i = 0; i < NumThreads; ++i)
	{
		DWORD n;
		GetExitCodeThread(handles[i], &n);
		exitCodeSum += n;
		number cycles;
		QueryThreadCycleTime(handles[i], &cycles);
		TotalCPUcycles += cycles;
		CloseHandle(handles[i]);
	}
	return exitCodeSum;
}