#include "pch.h"
#include "CppUnitTest.h"
#include "../pair_program/main.cpp"
#include "../pair_program/core.h"
#include "../pair_program/core.cpp"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mainTest
{
	TEST_CLASS(mainTest)
	{
	public:
		
		TEST_METHOD(missing_arguments)
		{
			try {
				char * args[] = { "C:\\Users\\Q\\Desktop\\test.txt" };
				main(1, args);
			}catch(invalid_argument const &e){
				Assert::AreEqual(0, strcmp("Missing function option, please choose -n or -w or -c", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(duplicated_wnc) {
			try {
				char * args[] = { "-w", "-n","-c","C:\\Users\\Q\\Desktop\\test.txt" };
				main(4, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Function option -w, -n and -c conflict", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(undefined_x) {
			try {
				char * args[] = { "-o", "C:\\Users\\Q\\Desktop\\test.txt" };
				main(2, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Undefined option '-x'", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(argument_pattern) {
			try {
				char * args[] = { "-h","word", "C:\\Users\\Q\\Desktop\\test.txt" };
				main(3, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Argument of option '-x' should be a single alphabet", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(conflict_hj) {
			try {
				char * args[] = { "-h","a", "-j","a","C:\\Users\\Q\\Desktop\\test.txt" };
				main(5, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Argument of -h and -j conflict. No answer", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(independent_n) {
			try {
				char * args[] = { "-n","-r","C:\\Users\\Q\\Desktop\\test.txt" };
				main(3, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("-n should be used independantly", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(file_notfind) {
			try {
				char * args[] = { "-n","C:\\Users\\Q\\Desktop\\fileee.txt" };
				main(2, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Can not find file", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(wrong_fileformat) {
			try {
				char * args[] = { "-n","wrong_fileformat.txt" };
				main(2, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Wrong file format", e.what()));
				return;
			}
			Assert::Fail();
		}
		TEST_METHOD(ring_detect) {
			char* words[101] = { "fddsu", "uasdasf", "ugfl", "laght", "adbon", "tasdu" };
			char* result[101];
			for (int i = 0; i < 101; i++)
			{
				result[i] = (char*)malloc(sizeof(char) * 601);
			}
			try {
				gen_chain_word(words, 6, result, 0, 0,0, false);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Ring dectected without -r option", e.what()));
				return;
			}
			for (int i = 0; i < 101; i++)
			{
				free(result[i]);
			}
			Assert::Fail();
		}
		TEST_METHOD(wordlen_overflow) {
			try {
				char * args[] = { "-n","wordlenoverflow.txt" };
				main(2, args);
			}
			catch (invalid_argument const &e) {
				Assert::AreEqual(0, strcmp("Too many word chains", e.what()));
				return;
			}
			Assert::Fail();
		}

	};
}
