//
// Created by 徐子航 on 2023/3/7.
//

#ifndef PAIRS_CORE_H
#define PAIRS_CORE_H

#endif //PAIRS_CORE_H

int process(char *wordList[], char *result[], int len,
            int type, bool letterSum, char head, char tail, char j);

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char j, bool enable_loop);

int gen_chains_all(char *words[], int len, char *result[]);

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char j, bool enable_loop);