import os
import time

# testcase_num = 11

# folderName = input ()
# testcase_num = int(input ())

folderNames = ['basic_1', 'basic_2', 'basic_3', 'basic_4', 'basic_5', 'basic_6', 'basic_extra', 'pressure_1_easy', 'pressure_1_hard', 'pressure_2_easy', 'pressure_2_hard', 'pressure_3_easy', 'pressure_3_hard']
testcase_nums = [1, 1, 5, 5, 10, 10, 10, 10, 10, 10, 10, 10, 10]

# folderNames = ['basic_1', 'basic_2', 'basic_3', 'basic_4', 'basic_5', 'basic_6', 'basic_extra', 'pressure_1_easy', 'pressure_2_easy', 'pressure_3_easy']
# testcase_nums = [1, 1, 5, 5, 10, 10, 10, 10, 10, 10]

# folderNames= ['basic_6']
# testcase_nums = [10]

def runForce():
    for case in range (len (folderNames)):
        folderName = folderNames[case]
        testcase_num = testcase_nums[case]
        os.system("rm *.dat")
        os.system("rm log.txt")
        for i in range(1, testcase_num + 1):
            print("brute-force", i, "...")
            os.system("./code < total_rollback/" + folderName + "/" + str(i) + ".in > force/" + str(i) + ".out")

def runRollback():
    os.system("./compile.sh")
    for case in range (len (folderNames)):
        folderName = folderNames[case]
        testcase_num = testcase_nums[case]
        print ("running " + folderName)
        os.system("rm *.dat")
        os.system("rm log.txt")
        os.system("mkdir test/" + folderName)
        t = time.time()
        for i in range(1, testcase_num + 1):
            print("rollback", i, "...")
            os.system("touch test/" + folderName + "/" + str(i) + ".out")
            curCaseT = time.time()
            os.system("./code < total_noRoll/" + folderName + "/" + str(i) + ".in > test/" + folderName + "/" + str(i) + ".out")
            curT = time.time() - curCaseT
            print(str(i) + ".in " + str(curT) + " seconds")
        tt = time.time() - t
        print("total time: " + str(tt) + " seconds")

def runEvaluate():
    for case in range (len (folderNames)):
        folderName = folderNames[case]
        testcase_num = testcase_nums[case]
        print("evaluate " + folderName)
        for i in range(1, testcase_num + 1):
            print("evaluate", i, "...")
            ret = os.system("diff test/" + folderName + "/" + str(i) + ".out" + " out/" + folderName + "/" + str(i) + ".out")
            if ret != 0:
                print("error in", i, "!")

# runForce()
runRollback()
# runEvaluate()