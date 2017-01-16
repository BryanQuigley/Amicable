#include "stdafx.h"
#include "PrimeTables.h"
#include "Engine.h"
#include "RangeGen.h"
#include "PGO_Helper.h"
#include "Tests.h"

PRAGMA_WARNING(push, 1)
PRAGMA_WARNING(disable : 4091 4917)
#include <boinc_api.h>
PRAGMA_WARNING(pop)

int main(int argc, char* argv[])
{
	BOINC_OPTIONS options;
	boinc_options_defaults(options);
	options.multi_thread = true;

	const int boinc_init_result = boinc_init_options(&options);
	if (boinc_init_result)
	{
		char buf[256];
		fprintf(stderr, "%s boinc_init returned %d\n", boinc_msg_prefix(buf, sizeof(buf)), boinc_init_result);
		exit(boinc_init_result);
	}

	boinc_fraction_done(0.0);

#if DYNAMIC_SEARCH_LIMIT
	if (argc < 2)
	{
		std::cerr << "You must specify search limit as a first command-line argument" << std::endl;
		return 0;
	}
	SearchLimit::value = static_cast<number>(StrToNumber(argv[1]));
	if (SearchLimit::value < 1000)
	{
		SearchLimit::value = 1000;
	}
	SearchLimit::LinearLimit = static_cast<number>(sqrt(SearchLimit::value * 2.0)) + 1;
	SearchLimit::MainPrimeTableBound = std::max<number>(SearchLimit::LinearLimit, 1000);
	SearchLimit::PrimeInversesBound = std::max<number>(static_cast<number>(sqrt(SearchLimit::value / 4)), CompileTimePrimes<CompileTimePrimesCount>::value);
	SearchLimit::SafeLimit = SearchLimit::value / 20;
#endif

	char* startFrom = nullptr;
	char* stopAt = nullptr;
	unsigned int largestPrimePower = 1;
	number startPrime = 0;
	number primeLimit = 0;

	// Parse command line parameters
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "/bench") == 0)
		{
			g_PrintNumbers = false;
		}

		if (strcmp(argv[i], "/instrument") == 0)
		{
			// Quickly generate profile data for all hot (most used) code paths
			PrimeTablesInit();
			ProfileGuidedOptimization_Instrument();
			return 0;
		}

		if (strcmp(argv[i], "/test") == 0)
		{
			PrimeTablesInit();
			g_PrintNumbers = false;

			std::cout << "Testing CheckPair()...";
			flush(std::cout);
			if (!TestCheckPair())
			{
				std::cerr << "CheckPair() test failed" << std::endl;
				return 1;
			}
			std::cout << "done" << std::endl << "Testing amicable candidates...";
			flush(std::cout);
			if (!TestAmicableCandidates())
			{
				std::cerr << "Amicable candidates test failed" << std::endl;
				return 2;
			}
			std::cout << "done" << std::endl << "Testing MaximumSumOfDivisors3()...";
			flush(std::cout);
			if (!TestMaximumSumOfDivisors3())
			{
				std::cerr << "MaximumSumOfDivisors3() test failed" << std::endl;
				return 3;
			}
			std::cout << "done" << std::endl << "Testing primesieve...";
			flush(std::cout);
			if (!TestPrimeSieve())
			{
				std::cerr << "primesieve test failed" << std::endl;
				return 4;
			}
			std::cout << "done" << std::endl << "All tests passed" << std::endl;
			return 0;
		}

		if ((strcmp(argv[i], "/from") == 0) && (i + 1 < argc))
		{
			startFrom = argv[++i];
		}

		if ((strcmp(argv[i], "/to") == 0) && (i + 1 < argc))
		{
			stopAt = argv[++i];
		}

		if (((strcmp(argv[i], "/largest_prime_power") == 0) || (strcmp(argv[i], "/lpp") == 0)) && (i + 1 < argc))
		{
			largestPrimePower = static_cast<unsigned int>(atoi(argv[++i]));
			if (largestPrimePower < 1)
			{
				largestPrimePower = 1;
			}
			if (largestPrimePower > 3)
			{
				largestPrimePower = 3;
			}
		}

		if (((strcmp(argv[i], "/large_primes_range") == 0) || (strcmp(argv[i], "/lpr") == 0)) && (i + 2 < argc))
		{
			startPrime = StrToNumber(argv[++i]);
			primeLimit = StrToNumber(argv[++i]);
		}

		if ((strcmp(argv[i], "/task_size") == 0) && (i + 1 < argc))
		{
			RangeGen::total_numbers_to_check = atof(argv[++i]);
		}
	}

	PrimeTablesInit((startPrime && primeLimit) || !stopAt);

	APP_INIT_DATA aid;
	boinc_get_init_data(aid);

	std::string resolved_name;
	const int boinc_resolve_result = boinc_resolve_filename_s("output.txt", resolved_name);
	if (boinc_resolve_result)
	{
		char buf[256];
		fprintf(stderr, "%s boinc_resolve_filename returned %d\n", boinc_msg_prefix(buf, sizeof(buf)), boinc_resolve_result);
		exit(boinc_resolve_result);
	}

	g_outputFile = boinc_fopen(resolved_name.c_str(), "ab+");
	RangeGen::Run(static_cast<number>(ceil(aid.ncpus)), startFrom, stopAt, largestPrimePower, startPrime, primeLimit);

	// Now sort all numbers found so far
	rewind(g_outputFile);
	std::vector<number> found_numbers;
	while (!feof(g_outputFile))
	{
		char buf[32];
		if (!fgets(buf, sizeof(buf), g_outputFile))
		{
			break;
		}
		const number m = StrToNumber(buf);
		if (m)
		{
			found_numbers.push_back(m);
		}
	}
	fclose(g_outputFile);

	std::sort(found_numbers.begin(), found_numbers.end());

	// Remove all duplicates
	found_numbers.erase(std::unique(found_numbers.begin(), found_numbers.end()), found_numbers.end());

	// And write remaining sorted numbers back to the output file
	g_outputFile = boinc_fopen(resolved_name.c_str(), "wb");
	for (number m : found_numbers)
	{
		fprintf(g_outputFile, "%llu\n", m);
	}
	fclose(g_outputFile);

	boinc_finish(0);
	return 0;
}
