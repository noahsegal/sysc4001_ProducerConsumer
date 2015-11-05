The following program was created for Carleton University's SYSC 4001 Operating Systems course (Fall 2015). It was written by Reid Cain-Mondoux and Noah Segal via peer programming. This program emulates the Producer and Consumer (P & C) problem outlined in our lecture. It utilizes shared memory between the P & C, and it is implemented with shared memory and reading/writing from/to files.

Noah Segal: 100911661
Reid Cain-Mondoux: 100945700

Supported Platform: Fedora (tested on CB-5109 Lab Computers)


This program must be run via a Unix Terminal:

1.1 cd into the desired directory and perform a "make clean" followed by "make"
        In Terminal Window (T1):
            T1$ make clean
            T1$ make

1.2.1 Test Consumer and Producer (with semaphore S):
        One Terminal Window (T1):
            T1$ ./producer & ./consumer
            
        Two Terminal Windows (T1 and T2):
            T1$ ./producer
            T2$ ./consumer

1.2.2 Test Consumer and Producer (without semaphore S):
        One Terminal Window (T1):
            T1$ ./producer_without_s & ./consumer_without_s
            
        Two Terminal Windows (T1 and T2):
            T1$ ./producer_without_s
            T2$ ./consumer_without_s

1.3 Terminal output will display the number of bytes written to / read from shared memory.
    Terminal output will also display the time it took the producer / consmer to run.
    
1.4 That's it. We hope that you enjoyed marking our assignment!


Included Files:
    consumer.c
    producer.c
    consumer_without_s.c
    producer_without_s.c
    config.h
    semun.h
    Makefile
    README.MD
    
Additional Files:
    PerformanceResults.txt
    input.txt
    100k.txt
    10K.txt



Additional Information:

Art by Rowan Crawford
               ________
          _,.-Y  |  |  Y-._
      .-~"   ||  |  |  |   "-.
      I" ""=="|" !""! "|"[]""|     _____
      L__  [] |..------|:   _[----I" .-{"-.
     I___|  ..| l______|l_ [__L]_[I_/r(=}=-P
    [L______L_[________]______j~  '-=c_]/=-^
     \_I_j.--.\==I|I==_/.--L_]
       [_((==)[`-----"](==)j
          I--I"~~"""~~"I--I
          |[]|         |[]|
          l__j         l__j
          |!!|         |!!|
          |..|         |..|
          ([])         ([])
          ]--[         ]--[
          [_L]         [_L]  -Row  (the Ascii-Wizard of Oz)
         /|..|\       /|..|\
        `=}--{='     `=}--{='
       .-^--r-^-.   .-^--r-^-.
       
http://www.ascii-code.com/ascii-art/movies/star-wars.php

