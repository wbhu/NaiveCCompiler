//
//  main.cpp
//  Compiler
//
//  Created by Leif Zhu on 11/05/2017.
//  Copyright © 2017 Leif Zhu. All rights reserved.
//

#include <iostream>
#include "SemanticAnalyzer.h"

int main()
{
    SemanticAnalyzer sa("2.txt");
    int es = sa.analyze(); //调语法分析
    if (es==0)  printf("语法分析成功!\n");
    else printf("语法分析错误!\n");
    return 0;
}
