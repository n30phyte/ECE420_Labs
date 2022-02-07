add_executable(Lab2_attacker Code/attacker.c)


#add_executable(Lab2_main2 Code/serialtester.c Code/IO.c)
#add_executable(Lab2_main3 Code/matrixgen.c)
#add_executable(Lab2_main4 Code/serialtester.c Code/IO.c)
#add_executable(Lab2_mainK Code/serialtester.c Code/IO.c)

target_link_libraries(Lab2_main1 Threads::Threads)