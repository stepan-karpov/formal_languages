## This folder in my github directory contains program which helps you to do nfa -> min pdfa transform
*Required language: C++20*

####Brief task implementation overview:
  - ##### src/nfa
    
    > This file contains 2 classes: NFA and DFA.
    > Each of this classes has it's own methods which you can use to simplify your work with automatons. Most of interface function has an expressive name, so I won't desrcibe them all.
    
    > To input the automaton you want, you need to follow the format: 
    > An example of an automaton description:

    ```
    2 3     - number of vertices, number of edges
    0 1 `   - there is an edge from vertex #0 to vertex #1 with a letter ` (eps)
    1 0 a   - there is an edge from vertex #1 to vertex #0 with a letter a
    0 0 a   - there is an edge from vertex #0 to vertex #0 with a letter a
    1       - the amount of terminal vertices
    0       - number of vertex itself (0-indexation)
    ```

    >the output will be represented in the same format with extended description

  - ##### tests/

    >As it was asked, this repository also contains test coverage.
    There are two tests (tests/test1-eps.txt, tests/test2-thompson.txt) which were checked manually

    >There is also an automation testing. To initialize it, write a proper way in the folder formal_languages/tests in tests/stresstestsC++.sh in line 2. Then simply run bash script. Be sure that you have g++ supporting c++20

    >I've configured automatically 10'000 tests, each test contain NFA and 1'000 words 0-15 length each. If a recognition on this set of 1'000 words stays the same after NFA-> min CDFA transform, then the test is passed. As you can see in log.txt file, the testing was done successfully and took ~30 minutes.

    >>correct_program.cpp     checks if there is possible to recognize a word with automaton

    >> create_test.cpp         creates test

    >> incorrect_program.cpp   completes NFA->min CDFA transform and checks recognition

    >> stresstestsC++.sh       initiates testing

  - ##### practical_work/

    >This file contains practical task implementation
    To use it, simply build repository with CMake and run ./practical file. Console interface supported.

    >There is also practical_work.cpp testing. To initialize it, write a proper way in the folder formal_languages/tests-practical in tests-practical/stresstestsC++.sh in line 2. Then simply run bash script. Be sure that you have g++ supporting c++20

    >I've configured automatically 100 tests, each test contain regex 'ab+c.aba.*.bac.+.+*' or 'acb..bab.c.*.ab.ba.+.+*a.', chars from 'abc' and random k from [1, 5]. If a recognition stays the same, then the test is passed. As you can see in log.txt file, the testing was done successfully and took ~0.5 minute.

    >>correct_program.cpp     checks if there is possible to find a proper word in a language, initiated by regex with brute-force algorithm

    >> create_test.cpp         creates test

    >> incorrect_program.cpp   checks if there is possible to find a proper word in a language, initiated by regex with graph algorithm

    >> stresstestsC++.sh       initiates testing


    



  