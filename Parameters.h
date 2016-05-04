#ifndef PARAMETERS_H
#define PARAMETERS_H

#pragma warning(disable: 4250)

#include <memory>

class PersonBase;
class TestHeuristicPerson;
class KnapsackSolver;

using PersonImpl = TestHeuristicPerson;
using KnapsackImpl = KnapsackSolver;

using ID_TYPE = unsigned long;
using GOOD_ID_TYPE = ID_TYPE;

// Compile parameters
constexpr static bool SHARED_PTR = false;

// Variables
constexpr static bool DRIVER_WAIT = false;
constexpr static bool CHECKPOINT_ENABLE = true;
constexpr static bool STOP_ON_SAME_TRANSITION_BEFORE = false;
constexpr static bool STOP_ON_MAX_ITERATION = true;
constexpr static unsigned short CHECKPOINT_TIME = 10;
constexpr static unsigned long MAX_ITERATION = 400;
constexpr static double KNAPSACK_FPTAS_EPSILON = 1 / 3;
constexpr static unsigned short MAX_PROP_ITERATION = 1;
constexpr static unsigned short WTP_ADJUST = 1;
constexpr static bool CLEAR_LOG = true;
constexpr static bool DEBUG_OUT = true;
constexpr static bool RESULTS_OUT = true;

// Text
constexpr static char* DEFAULT_PATH = "C:\\Users\\William\\Dropbox\\victoryLap\\thesis\\results\\inF.xml";
constexpr static char* LOG_PATH = "C:\\Users\\William\\Dropbox\\victoryLap\\thesis\\results\\logF2b.csv";
constexpr static char* RESULTS_PATH = "C:\\Users\\William\\Dropbox\\victoryLap\\thesis\\results\\outF2b.csv";

//constexpr static char* DEFAULT_PATH = "C:\\Users\\Eta Kappa Nu\\Documents\\MicroWorld\\inF.xml";
//constexpr static char* LOG_PATH = "C:\\Users\\Eta Kappa Nu\\Documents\\MicroWorld\\logF5hkn.csv";
//constexpr static char* RESULTS_PATH = "C:\\Users\\Eta Kappa Nu\\Documents\\MicroWorld\\outF5hkn.csv";

constexpr static char* PROMPT_TEXT = "Welcome to MicroWorld.\n";

// Characters
constexpr static char HOLDINGS_DELIMITER = '~';

// Pointer types
template<typename T, bool x> struct Ptr {
	using type = T*;
};

template<typename T> struct Ptr<T, true> {
	using type = std::shared_ptr<T>;
};

template<typename T> using Pointer = typename Ptr<T, SHARED_PTR>::type;

#endif
