#!/bin/bash


# Sean Szumlanski
# COP 3502, Spring 2018

# ===========================
# TriePrediction: test-all.sh
# ===========================
# You can run this script at the command line like so:
#
#   bash test-all.sh
#
# For more details, see the assignment PDF.


################################################################################
# Shell check.
################################################################################

# Running this script with sh instead of bash can lead to false positives on the
# test cases. Yikes! These checks ensure the script is not being run through the
# Bourne shell (or any shell other than bash).

if [ "$BASH" != "/bin/bash" ]; then
  echo ""
  echo " Bloop! Please use bash to run this script, like so: bash test-all.sh"
  echo ""
  exit
fi

if [ -z "$BASH_VERSION" ]; then
  echo ""
  echo " Bloop! Please use bash to run this script, like so: bash test-all.sh"
  echo ""
  exit
fi


################################################################################
# Initialization.
################################################################################

PASS_CNT=0
NUM_INPUT_TESTS=9
NUM_UNIT_TESTS=7
NUM_TEST_CASES=`expr $NUM_INPUT_TESTS + $NUM_UNIT_TESTS`

FIRST_UNIT_TEST=`expr $NUM_INPUT_TESTS + 1`
FINAL_UNIT_TEST=`expr $NUM_INPUT_TESTS + $NUM_UNIT_TESTS`


################################################################################
# Magical incantations.
################################################################################

# Ensure that obnoxious glibc errors are piped to stderr.
export LIBC_FATAL_STDERR_=1

# Now redirect all local error messages to /dev/null (like "process aborted").
exec 2> /dev/null


################################################################################
# Check that all required files are present.
################################################################################

if [ ! -f TriePrediction.c ]; then
	echo ""
	echo " Error: You must place TriePrediction.c in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -f TriePrediction.h ]; then
	echo ""
	echo " Error: You must place TriePrediction.h in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -f UnitTestLauncher.c ]; then
	echo ""
	echo " Error: You must place UnitTestLauncher.c in this directory before"
	echo "        we can proceed. Aborting test script."
	echo ""
	exit
elif [ ! -d sample_output ]; then
	echo ""
	echo " Error: You must place the sample_output folder in this directory"
	echo "        before we can proceed. Aborting test script."
	echo ""
	exit
fi

check_test_case()
{
	local filename=$1

	if [ ! -f $filename ]; then
		echo ""
		echo " Error: You must place $filename in this directory"
		echo "        before we can proceed. Aborting test script."
		echo ""
		exit
	fi
}

check_sample_output()
{
	local filename=$1

	if [ ! -f sample_output/$filename ]; then
		echo ""
		echo " Error: You must place $filename in the"
		echo "        sample_output directory before we can proceed."
		echo "        Aborting test script."
		echo ""
		exit
	fi
}

for i in `seq -f "%02g" 1 $NUM_INPUT_CASES`;
do
	check_test_case "corpus$i.txt"
	check_test_case "input$i.txt"
	check_sample_output "output$i.txt"
done

for i in `seq -f "%02g" $FIRST_UNIT_TEST $FINAL_UNIT_TEST`;
do
	check_test_case "unit_test$i.c"
	check_sample_output "unit_test$i-output.txt"
done

# Save a backup version of TriePrediction.h to be restored after running this
# script, so that the #define main line ends up back how you had it.
cp TriePrediction.h TriePrediction_backup.h


################################################################################
# Run test cases with input specified at command line (standard test cases).
################################################################################

echo ""
echo "================================================================"
echo "Running input-based test cases..."
echo "================================================================"
echo ""

# Function for running a single test case.
run_test_case()
{
	local corpus_file=$1
	local input_file=$2
	local output_file=$3

	echo -n "  [Test Case] ./a.out $corpus_file $input_file ... "

	# Ensure that TriePrediction.h is in non-unit test case mode.
	perl -p -i -e 's/^#define main/\/\/#define main/' TriePrediction.h

	# Attempt to compile.
	gcc TriePrediction.c 2> /dev/null
	compile_val=$?
	if [[ $compile_val != 0 ]]; then
		echo "fail (failed to compile)"
		continue
	fi

	# Run program. Capture return value to check whether it crashes.
	./a.out $corpus_file $input_file > myoutput.txt 2> /dev/null
	execution_val=$?
	if [[ $execution_val != 0 ]]; then
		echo "fail (program crashed)"
		continue
	fi

	# Run diff and capture its return value.
	diff myoutput.txt sample_output/$output_file > /dev/null
	diff_val=$?
	
	# Output results based on diff's return value.
	if  [[ $diff_val != 0 ]]; then
		echo "fail (output mismatch)"
	else
		echo "PASS!"
		PASS_CNT=`expr $PASS_CNT + 1`
	fi
}

for i in `seq -f "%02g" 1 $NUM_INPUT_TESTS`;
do
	run_test_case "corpus$i.txt" "input$i.txt" "output$i.txt" 2> /dev/null
done


################################################################################
# Run unit tests.
################################################################################

echo ""
echo "================================================================"
echo "Running unit test cases..."
echo "================================================================"
echo ""

run_unit_test()
{
	local testcase_file=$1
	local output_file=$2

	echo -n "  [Unit Test] $testcase_file ... "

	# Ensure that TriePrediction.h is in unit test case mode.
	perl -p -i -e 's/^\/\/(\s*)#define main/#define main/' TriePrediction.h

	# Attempt to compile.
	gcc TriePrediction.c UnitTestLauncher.c $testcase_file 2> /dev/null
	compile_val=$?
	if [[ $compile_val != 0 ]]; then
		echo "fail (failed to compile)"
		continue
	fi

	# Run program. Capture return value to check whether it crashes.
	./a.out > myoutput.txt 2> /dev/null
	execution_val=$?
	if [[ $execution_val != 0 ]]; then
		echo "fail (program crashed)"
		continue
	fi

	# Run diff and capture its return value.
	diff myoutput.txt sample_output/$output_file > /dev/null
	diff_val=$?
	
	# Output results based on diff's return value.
	if  [[ $diff_val != 0 ]]; then
		echo "fail (output mismatch)"
	else
		echo "PASS!"
		PASS_CNT=`expr $PASS_CNT + 1`
	fi
}

for i in `seq -f "%02g" $FIRST_UNIT_TEST $FINAL_UNIT_TEST`;
do
	run_unit_test "unit_test$i.c" "unit_test$i-output.txt" 2> /dev/null
done


################################################################################
# Cleanup phase.
################################################################################

# Restore TriePrediction.h to whatever mode it was in before running this script.
if [ -f TriePrediction_backup.h ]; then
	mv TriePrediction_backup.h TriePrediction.h
fi

# Clean up the executable file.
rm -f a.out
rm -f myoutput.txt


################################################################################
# Final thoughts.
################################################################################

echo ""
echo "================================================================"
echo "Final Report"
echo "================================================================"

if [ $PASS_CNT -eq $NUM_TEST_CASES ]; then
	echo ""
	echo "              ,)))))))),,,"
	echo "            ,(((((((((((((((,"
	echo "            )\\\`\\)))))))))))))),"
	echo "     *--===///\`_    \`\`\`((((((((("
	echo "           \\\\\\ b\\  \\    \`\`)))))))"
	echo "            ))\\    |     ((((((((               ,,,,"
	echo "           (   \\   |\`.    ))))))))       ____ ,)))))),"
	echo "                \\, /  )  ((((((((-.___.-\"    \`\"((((((("
	echo "                 \`\"  /    )))))))               \\\`)))))"
	echo "                    /    ((((\`\`                  \\((((("
	echo "              _____|      \`))         /          |)))))"
	echo "             /     \\                 |          / ((((("
	echo "            /  --.__)      /        _\\         /   )))))"
	echo "           /  /    /     /'\`\"~----~\`  \`.       \\   (((("
	echo "          /  /    /     /\`              \`-._    \`-. \`)))"
	echo "         /_ (    /    /\`                    \`-._   \\ (("
	echo "        /__|\`   /   /\`                        \`\\\`-. \\ ')"
	echo "               /  /\`                            \`\\ \\ \\"
	echo "              /  /                                \\ \\ \\"
	echo "             /_ (                                 /_()_("
	echo "            /__|\`                                /__/__|"
	echo ""
	echo "                             Legendary."
	echo ""
	echo "                10/10 would run this program again."
	echo ""
	echo "  CONGRATULATIONS! You appear to be passing all required test"
	echo "  cases! (Now, don't forget to create some extra test cases of"
	echo "  your own. These test cases are not necessarily comprehensive.)"
	echo ""
	echo "================================================================"
	echo "Percy Shelley's \"Ozymandias\" (from corpus07.txt)"
	echo "================================================================"
	echo ""
	echo "             I met a traveller from an antique land"
	echo "         Who said: Two vast and trunkless legs of stone"
	echo "         Stand in the desert... near them, on the sand,"
	echo "        Half sunk, a shattered visage lies, whose frown,"
	echo "          And wrinkled lip, and sneer of cold command,"
	echo "        Tell that its sculptor well those passions read"
	echo "       Which yet survive, stamped on these lifeless things,"
	echo "        The hand that mocked them and the heart that fed:"
	echo ""
	echo "             And on the pedestal these words appear:"
	echo "             'My name is Ozymandias, king of kings:"
	echo "            Look on my works, ye Mighty, and despair!'"
	echo "             Nothing beside remains. Round the decay"
	echo "           Of that colossal wreck, boundless and bare"
	echo "           The lone and level sands stretch far away."
	echo ""
else
	echo "                           ."
	echo "                          \":\""
	echo "                        ___:____     |\"\\/\"|"
	echo "                      ,'        \`.    \\  /"
	echo "                      |  o        \\___/  |"
	echo "                    ~^~^~^~^~^~^~^~^~^~^~^~^~"
	echo ""
	echo "                           (fail whale)"
	echo ""
	echo "  The fail whale is friendly and adorable! He is not here to"
	echo "  demoralize you, but rather, to bring you comfort and joy"
	echo "  in your time of need. \"Keep plugging away,\" he says! \"You"
	echo "  can do this!\""
	echo ""
	echo "  For instructions on how to run these test cases individually"
	echo "  and inspect how your output differs from the expected output,"
	echo "  be sure to consult the assignment PDF."
	echo ""
fi
