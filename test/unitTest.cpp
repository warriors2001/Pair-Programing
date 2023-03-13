#include "pch.h"
#include  "CppUnitTest.h"
#include "../pair_program/core.h"
#include "../pair_program/core.cpp"
#include  <iostream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unitTest
{
	void test_gen_chain_word( char* words[], int len,  char* ans[], int ans_len, char head, char tail,char forbid, bool enable_loop) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_word(words, len, result, head, tail, forbid, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
		}
	}
	void test_gen_chain_char(char* words[], int len, char* ans[], int ans_len, char head, char tail, char forbid, bool enable_loop) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chain_char(words, len, result, head, tail, forbid, enable_loop);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			for (int i = 0; i < ans_len; i++) {
				if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
			}
		}
	}
	void test_gen_chains_all( char* words[], int len,  char* ans[], int ans_len) {
		char** result = (char**)malloc(10000);
		int out_len = gen_chains_all(words, len, result);
		Assert::AreEqual(ans_len, out_len);
		for (int i = 0; i < ans_len; i++) {
			if (result != nullptr) Assert::AreEqual(strcmp(ans[i], result[i]), 0);
		}
	}
	TEST_CLASS(unitTest)
	{
	public:
		/*
		-w
		*/
		TEST_METHOD(test_w)
		{
			 char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			 char* ans[] = { "algebra", "apple", "elephant", "trick" };
			 test_gen_chain_word(words, 11, ans, 4, 0, 0, 0,false);
		}
		/*
		-w -h
		*/
		TEST_METHOD(test_w_h) {
			 char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			 char* ans[] = { "elephant", "trick" };
			 test_gen_chain_word(words, 11, ans, 2, 'e', 0, 0,false);
		}
		/*
		-w -t
		*/
		TEST_METHOD(test_w_t) {
			 char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			 char* ans[] = { "algebra", "apple", "elephant" };
			test_gen_chain_word(words, 11, ans, 3, 0, 't',0, false);
		}
		/*
		-w -j
		*/
		TEST_METHOD(test_w_j) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "elephant", "trick"};
			test_gen_chain_word(words, 11, ans, 2, 0, 0,'a', false);
		}
		/*
		-w -h -t
		*/
		TEST_METHOD(test_w_h_t) {
			 char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			 char* ans[] = { "algebra", "apple", "elephant" };
			 test_gen_chain_word(words, 11, ans, 3, 'a', 't', 0, false);
		}
		/*
		-w -h -j
		*/
		TEST_METHOD(test_w_h_j) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "algebra", "apple" };
			test_gen_chain_word(words, 11, ans, 2, 'a', 0, 'e', false);
		}
		/*
		-w -t -j
		*/
		TEST_METHOD(test_w_t_j) {
			char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			char* ans[] = { "elephant", "trick" };
			test_gen_chain_word(words, 11, ans, 2, 0, 'k', 'a', false);
		}
		/*
		-w -h -t -j 
		*/
		TEST_METHOD(test_w_h_t_j) {
			char* words[] = {"asdac","jiasdnc","qweudasunc","casdowdn","nasdnw" ,"nqwer","rhusad","radqwt","tqwdbf","dqwdf"};
			char* ans[] = { "qweudasunc","casdowdn" ,"nqwer" ,"rhusad","dqwdf" };
			test_gen_chain_word(words, 10, ans, 5,'q', 'f', 't', false);
		}

		/*
		* 4 ���� 6 ���ߵ� DAG
		*/
		TEST_METHOD(max_DAG_4_vertices) {
			 char* words[] = { "ab", "ac", "ad", "bc", "bd", "cd" };
			 char* ans[] = { "ab", "bc", "cd" };
			 test_gen_chain_word(words, 6, ans, 3, 0, 0,0, false);
		}

		/*
		* 3 ������� + 3 ���Ի�
		*/
		TEST_METHOD(simple_chain_with_self_loop) {
			 char* words[] = { "aa", "ab", "bb", "bc", "cc" };
			 char* ans[] = { "aa", "ab", "bb", "bc", "cc" };
			 test_gen_chain_word(words, 5, ans, 5, 0, 0, 0, false);
		}

		/*
		* 3 ������� + 3 ���Ի� vs 4 �������
		*/
		TEST_METHOD(simple_chain_with_self_loop_vs_simple_chain) {
			 char* words[] = { "aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh" };
			 char* ans[] = { "aa", "ab", "bb", "bc", "cc" };
			 test_gen_chain_word(words, 9, ans, 5, 0, 0,0, false);
		}

		/*
* 5 个孤立自环点
*/
		TEST_METHOD(isolated_vertex_with_self_loop) {
			 char* words[] = { "aa", "bb", "cc", "dd", "ee" };
			 char* ans[] = { "none" };
			test_gen_chain_word(words, 5, ans, 0, 0, 0,0, false);
		}

		/*
		* ����Ϊ 5 ���� ����һЩ������ĸ�ĵ���
		*/
		TEST_METHOD(simple_chain_and_single_character) {
			 char* words[] = { "ab", "bc", "cd", "de", "ea", "a", "b", "c", "d", "e" };
			 char* ans[] = { "ab", "bc", "cd", "de", "ea" };
			 test_gen_chain_word(words, 10, ans, 5, 0, 0,0, true);
		}

		/*
		* -r ����
		*/
		TEST_METHOD(example_r) {
			 char* words[] = { "element", "heaven", "table", "teach", "talk" };
			 char* ans[] = { "table", "element", "teach", "heaven" };
			 test_gen_chain_word(words, 5, ans, 4, 0, 0, 0, true);
		}

		/*
		* 10 �� aa
		*/
		TEST_METHOD(only_self_loop_aa) {
			 char* words[] = { "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa", "aa" };
			 char* ans[] = { "xx" };
			 test_gen_chain_word(words, 10, ans, 0, 0, 0, 0, true);
		}
		//============================gen_chain_char====================
		/*
		-c
		*/
		TEST_METHOD(test_c) {
			 char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
			 char* ans[] = { "pseudopseudohypoparathyroidism", "moon" };
			test_gen_chain_char(words, 11, ans, 2, 0, 0,0, false);
		}
		//==========================gen_chains_all=====================
		/*
		* -n ����
		*/
		TEST_METHOD(example_n) {
			 char* words[] = { "woo", "oom", "moon", "noox" };
			 char* ans[] = { "moon noox", "oom moon", "oom moon noox", "woo oom", "woo oom moon", "woo oom moon noox" };
			test_gen_chains_all(words, 4, ans, 6);
		}

	};
}
