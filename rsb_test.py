from typing import Callable
import unittest
import subprocess
import random

ADDMULGRAY_PARAMS = [
    0,
    1,
    2,
    3,
    8,
    21,
    55,
    144,
    377,
    987,
    2584,
    6765,
    17711,
    46368,
    121393,
    317811,
    832040,
    2178309,
    5702887,
    14930352,
    39088169,
    102334155,
    267914296,
    701408733,
    1836311903,
    4294967295
]

RPN_EVAL_SYNCHECK = [
    "",
    "A",
    "!",
    "&",
    "|",
    "^",
    ">",
    "=",
    "0&",
    "0|",
    "0^",
    "0>",
    "0=",
    "01",
    "01!",
    "01|10&",
    "0|1",
    "0 1&",
    "02&",
    "01=11>^00|1&!"
]

RPN_VAR_SYNCHECK = [
    "",
    "0",
    "1",
    "!",
    "&",
    "|",
    "^",
    ">",
    "=",
    "A&",
    "A|",
    "A^",
    "A>",
    "A=",
    "AB",
    "AB!",
    "AB|BA&",
    "A|B",
    "A B&",
    "AB=CD>^EF|G&!"
]

def bit_self(a: bool) -> bool:
    return a

def bit_not(a: bool) -> bool:
    return not a

def bit_and(a: bool, b: bool) -> bool:
    return a & b

def bit_or(a: bool, b: bool) -> bool:
    return a | b

def bit_xor(a: bool, b: bool) -> bool:
    return a ^ b

def bit_imply(a: bool, b: bool) -> bool:
    return (not a) | b

def bit_equiv(a: bool, b: bool) -> bool:
    return a == b

RPN_EVAL_SIMPLE: list[tuple[str, Callable[[bool], bool]]] = [
    ("", bit_self),
    ("!", bit_not)
]

RPN_EVAL_DOUBLE: list[tuple[str, Callable[[bool, bool], bool]]] = [
    ("&", bit_and),
    ("|", bit_or),
    ("^", bit_xor),
    (">", bit_imply),
    ("=", bit_equiv)
]

RPN_EVAL_RESULT = [
    "False",
    "True"
]

RPN_FORM_CHECK_SIMPLE = [
    "A",
    "A!",
    "AB&",
    "AB|",
    "AB^",
    "AB>",
    "AB=",
]

RPN_FORM_CHECK_COMPLEX = [
    "AA&",
    "AA|",
    "AA^",
    "AA!&",
    "AA!>",
    "AB&!",
    "AB|!",
    "AB^!",
    "AB>!",
    "AB=!",
    "A!B&",
    "B!A|",
    "AAA^^",
    "AA>A&",
    "ABC&&",
    "ABC&|",
    "ABC|&",
    "ABC||",
    "ABC^^",
    "ABC>>",
    "ABC==",
    "AB^A&",
    "AB>C>",
    "AB=A&",
    "ABC&|!",
    "ABC||!",
    "B!A|C|",
    "ABCD^^^",
    "AB^A&B&",
    "AB>BA>&",
    "AB>A>A>",
    "AD&BC&^",
    "AB&CD&&!",
    "AB|CD|&!",
    "AB|CD||!",
    "B!A|C|Q&",
    "ABCDE&&&&",
    "AB&A!B!&&",
    "AB&A!B!&|",
    "B!A|C|Q&Y|",
    "AB>BA>&AB==",
    "AB=CD>^EF|G&&!",
    "AB|CD|&AC|BD|&|!",
]

class NumbersTestResult(unittest.TextTestResult):
    def addSubTest(self, test, subtest, outcome):
        # handle failures calling base class
        super(NumbersTestResult, self).addSubTest(test, subtest, outcome)
        # add to total number of tests run
        self.testsRun += 1

class TestEx00(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex00/adder", "ex00/adder.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self):
        proc = subprocess.run([
            "rm", "-f", "ex00/adder"
        ])

    def test_adder(self):
        for lhs in ADDMULGRAY_PARAMS:
            for rhs in ADDMULGRAY_PARAMS:
                with self.subTest(lhs=lhs, rhs=rhs):
                    proc = subprocess.run(["./ex00/adder", str(lhs), str(rhs)], stdout=subprocess.PIPE, text=True)

                    self.assertEqual(proc.returncode, 0)
                    self.assertEqual(str((lhs + rhs) & 0xFFFFFFFF), proc.stdout.strip())

class TestEx01(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex01/multiplier", "ex01/multiplier.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self):
        proc = subprocess.run([
            "rm", "-f", "ex01/multiplier"
        ])

    def test_multiplier(self):
        for lhs in ADDMULGRAY_PARAMS:
            for rhs in ADDMULGRAY_PARAMS:
                with self.subTest(lhs=lhs, rhs=rhs):
                    proc = subprocess.run(["./ex01/multiplier", str(lhs), str(rhs)], stdout=subprocess.PIPE, text=True)

                    self.assertEqual(proc.returncode, 0)
                    self.assertEqual(str((lhs * rhs) & 0xFFFFFFFF), proc.stdout.strip())

class TestEx02(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex02/gray", "ex02/gray.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self):
        proc = subprocess.run([
            "rm", "-f", "ex02/gray"
        ])

    def test_gray(self):
        for lhs in ADDMULGRAY_PARAMS:
            with self.subTest(lhs=lhs):
                proc = subprocess.run(["./ex02/gray", str(lhs)], stdout=subprocess.PIPE, text=True)

                self.assertEqual(proc.returncode, 0)
                self.assertEqual(str(lhs ^ (lhs >> 1)), proc.stdout.strip())

class TestEx03(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex03/eval_formula", "ex03/eval_formula.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self) -> None:
        proc = subprocess.run([
            "rm", "-f", "ex03/eval_formula"
        ])

    def test_eval_formula_00_invalid(self):
        for rpn in RPN_EVAL_SYNCHECK:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex03/eval_formula", rpn], stdout=subprocess.PIPE, text=True)

                line_list = proc.stdout.split("\n")
                line_list = [l for l in filter(lambda s: len(s) != 0, line_list)]

                self.assertEqual(proc.returncode, 0)
                self.assertEqual(len(line_list), 2)
                self.assertEqual("False", line_list[1])

    def test_eval_formula_01_simple(self):
        for value in range(2):
            valA = bool((value >> 0) & 1)

            for eval_tuple in RPN_EVAL_SIMPLE:
                rpn = "01"[valA] + eval_tuple[0]
                result = eval_tuple[1](valA)

                with self.subTest(rpn=rpn):
                    proc = subprocess.run(["./ex03/eval_formula", rpn], stdout=subprocess.PIPE, text=True)

                    eval_str = proc.stdout.strip()

                    self.assertIn(eval_str, RPN_EVAL_RESULT)
                    eval_bool = bool(RPN_EVAL_RESULT.index(eval_str))
                    self.assertEqual(eval_bool, result)

    def test_eval_formula_02_double(self):
        for value in range(4):
            valA = bool((value >> 0) & 1)
            valB = bool((value >> 1) & 1)

            for eval_tuple in RPN_EVAL_DOUBLE:
                rpn = "01"[valA] + "01"[valB] + eval_tuple[0]
                result = eval_tuple[1](valA, valB)

                with self.subTest(rpn=rpn):
                    proc = subprocess.run(["./ex03/eval_formula", rpn], stdout=subprocess.PIPE, text=True)

                    eval_str = proc.stdout.strip()

                    self.assertIn(eval_str, RPN_EVAL_RESULT)
                    eval_bool = bool(RPN_EVAL_RESULT.index(eval_str))
                    self.assertEqual(eval_bool, result)

    def test_eval_formula_03_composite(self):
        for value in range(8):
            valA = bool((value >> 0) & 1)
            valB = bool((value >> 1) & 1)
            valC = bool((value >> 2) & 1)

            for opvalue in range((len(RPN_EVAL_DOUBLE) ** 2) * (2 ** 5)):
                o1 = (opvalue >> 5) % len(RPN_EVAL_DOUBLE)
                o2 = (opvalue >> 5) // len(RPN_EVAL_DOUBLE)
                n1 = (opvalue >> 0) & 1
                n2 = (opvalue >> 1) & 1
                n3 = (opvalue >> 2) & 1
                n4 = (opvalue >> 3) & 1
                n5 = (opvalue >> 4) & 1

                eto1 = RPN_EVAL_DOUBLE[o1]
                eto2 = RPN_EVAL_DOUBLE[o2]
                etn1 = RPN_EVAL_SIMPLE[n1]
                etn2 = RPN_EVAL_SIMPLE[n2]
                etn3 = RPN_EVAL_SIMPLE[n3]
                etn4 = RPN_EVAL_SIMPLE[n4]
                etn5 = RPN_EVAL_SIMPLE[n5]

                rpn = "01"[valA] + etn1[0] + "01"[valB] + etn2[0] + eto1[0] + etn3[0] + "01"[valC] + etn4[0] + eto2[0] + etn5[0]
                result = etn5[1](eto2[1](etn3[1](eto1[1](etn1[1](valA), etn2[1](valB))), etn4[1](valC)))

                with self.subTest(rpn=rpn):
                    proc = subprocess.run(["./ex03/eval_formula", rpn], stdout=subprocess.PIPE, text=True)

                    eval_str = proc.stdout.strip()

                    self.assertIn(eval_str, RPN_EVAL_RESULT)
                    eval_bool = bool(RPN_EVAL_RESULT.index(eval_str))
                    self.assertEqual(eval_bool, result)

class TestEx04(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex04/print_truth_table", "ex04/print_truth_table.cpp", "ex04/rpn_tree.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self) -> None:
        proc = subprocess.run([
            "rm", "-f", "ex04/print_truth_table"
        ])

    def test_ptt_00_invalid(self):
        for rpn in RPN_VAR_SYNCHECK:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(proc.returncode, 0)
                self.assertEqual(proc.stdout.strip(), "Invalid RPN string.")

    def test_ptt_01_simple(self):
        for eval_tuple in RPN_EVAL_SIMPLE:
            rpn = "A" + eval_tuple[0]
            proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)

            line_table = proc.stdout.split("\n")[:-1]

            with self.subTest(rpn=rpn):
                self.assertEqual(len(line_table), 4)
                self.assertEqual(line_table[0], "| A | = |")
                self.assertEqual(line_table[1], "|---|---|")

            for value in range(2):
                valA = bool((value >> 0) & 1)

                with self.subTest(rpn=rpn, a=valA):
                    result = eval_tuple[1](valA)
                    expected_line = "| %d | %d |" % (int(valA), int(result))
                    self.assertEqual(line_table[value + 2], expected_line)

    def test_ptt_02_double(self):
        for eval_tuple in RPN_EVAL_DOUBLE:
            rpn = "AB" + eval_tuple[0]
            proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)

            line_table = proc.stdout.split("\n")[:-1]

            with self.subTest(rpn=rpn):
                self.assertEqual(len(line_table), 6)
                self.assertEqual(line_table[0], "| A | B | = |")
                self.assertEqual(line_table[1], "|---|---|---|")

            for value in range(4):
                valA = bool((value >> 1) & 1)
                valB = bool((value >> 0) & 1)

                with self.subTest(rpn=rpn, a=valA, b=valB):
                    result = eval_tuple[1](valA, valB)
                    expected_line = "| %d | %d | %d |" % (int(valA), int(valB), int(result))
                    self.assertEqual(line_table[value + 2], expected_line)

    def test_ptt_03_composite(self):
        for opvalue in range((len(RPN_EVAL_DOUBLE) ** 2) * (2 ** 5)):
            o1 = (opvalue >> 5) % len(RPN_EVAL_DOUBLE)
            o2 = (opvalue >> 5) // len(RPN_EVAL_DOUBLE)
            n1 = (opvalue >> 0) & 1
            n2 = (opvalue >> 1) & 1
            n3 = (opvalue >> 2) & 1
            n4 = (opvalue >> 3) & 1
            n5 = (opvalue >> 4) & 1

            eto1 = RPN_EVAL_DOUBLE[o1]
            eto2 = RPN_EVAL_DOUBLE[o2]
            etn1 = RPN_EVAL_SIMPLE[n1]
            etn2 = RPN_EVAL_SIMPLE[n2]
            etn3 = RPN_EVAL_SIMPLE[n3]
            etn4 = RPN_EVAL_SIMPLE[n4]
            etn5 = RPN_EVAL_SIMPLE[n5]

            rpn = "A" + etn1[0] + "B" + etn2[0] + eto1[0] + etn3[0] + "C" + etn4[0] + eto2[0] + etn5[0]
            proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)

            line_table = proc.stdout.split("\n")[:-1]

            with self.subTest(rpn=rpn):
                self.assertEqual(len(line_table), 10)
                self.assertEqual(line_table[0], "| A | B | C | = |")
                self.assertEqual(line_table[1], "|---|---|---|---|")

            for value in range(8):
                valA = bool((value >> 2) & 1)
                valB = bool((value >> 1) & 1)
                valC = bool((value >> 0) & 1)

                with self.subTest(rpn=rpn, a=valA, b=valB, c=valC):
                    result = etn5[1](eto2[1](etn3[1](eto1[1](etn1[1](valA), etn2[1](valB))), etn4[1](valC)))
                    expected_line = "| %d | %d | %d | %d |" % (int(valA), int(valB), int(valC), int(result))
                    self.assertEqual(line_table[value + 2], expected_line)

class TestEx05(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex05/negation_normal_form", "ex05/negation_normal_form.cpp", "ex05/rpn_tree.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex04/print_truth_table", "ex04/print_truth_table.cpp", "ex04/rpn_tree.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self) -> None:
        proc = subprocess.run([
            "rm", "-f", "ex05/negation_normal_form"
        ])
        proc = subprocess.run([
            "rm", "-f", "ex04/print_truth_table"
        ])


    def test_nnf_00_invalid(self):
        for rpn in RPN_VAR_SYNCHECK:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex05/negation_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(proc.returncode, 0)
                self.assertEqual(proc.stdout.strip(), "Invalid RPN string.")

    def test_nnf_01_simple(self):
        for rpn in RPN_FORM_CHECK_SIMPLE:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex05/negation_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                nnf_rpn = proc.stdout.strip()

                for i in range(len(nnf_rpn)):
                    if nnf_rpn[i].isupper():
                        pass
                    elif nnf_rpn[i] == "!":
                        self.assertGreater(i, 0)
                        self.assertTrue(nnf_rpn[i - 1].isupper())
                    else:
                        self.assertIn(nnf_rpn[i], "&|")

                rpn_proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)
                nnf_proc = subprocess.run(["./ex04/print_truth_table", nnf_rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(rpn_proc.stdout, nnf_proc.stdout)

    def test_nnf_02_complex(self):
        for rpn in RPN_FORM_CHECK_COMPLEX:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex05/negation_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                nnf_rpn = proc.stdout.strip()

                for i in range(len(nnf_rpn)):
                    if nnf_rpn[i].isupper():
                        pass
                    elif nnf_rpn[i] == "!":
                        self.assertGreater(i, 0)
                        self.assertTrue(nnf_rpn[i - 1].isupper())
                    else:
                        self.assertIn(nnf_rpn[i], "&|")

                rpn_proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)
                nnf_proc = subprocess.run(["./ex04/print_truth_table", nnf_rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(rpn_proc.stdout, nnf_proc.stdout)

class TestEx06(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex06/conjunctive_normal_form", "ex06/conjunctive_normal_form.cpp", "ex06/rpn_tree.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex04/print_truth_table", "ex04/print_truth_table.cpp", "ex04/rpn_tree.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self) -> None:
        proc = subprocess.run([
            "rm", "-f", "ex06/conjunctive_normal_form"
        ])
        proc = subprocess.run([
            "rm", "-f", "ex04/print_truth_table"
        ])


    def test_cnf_00_invalid(self):
        for rpn in RPN_VAR_SYNCHECK:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex06/conjunctive_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(proc.returncode, 0)
                self.assertEqual(proc.stdout.strip(), "Invalid RPN string.")

    def test_cnf_01_simple(self):
        for rpn in RPN_FORM_CHECK_SIMPLE:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex06/conjunctive_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                nnf_rpn = proc.stdout.strip()

                for i in range(len(nnf_rpn)):
                    if nnf_rpn[i].isupper():
                        pass
                    elif nnf_rpn[i] == "!":
                        self.assertGreater(i, 0)
                        self.assertTrue(nnf_rpn[i - 1].isupper())
                    else:
                        self.assertIn(nnf_rpn[i], "&|")

                rpn_proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)
                nnf_proc = subprocess.run(["./ex04/print_truth_table", nnf_rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(rpn_proc.stdout, nnf_proc.stdout)

    def test_cnf_02_complex(self):
        for rpn in RPN_FORM_CHECK_COMPLEX:
            with self.subTest(rpn=rpn):
                proc = subprocess.run(["./ex06/conjunctive_normal_form", rpn], stdout=subprocess.PIPE, text=True)

                cnf_rpn = proc.stdout.strip()

                self.assertNotIn("&", cnf_rpn.rstrip("&"))

                for i in range(len(cnf_rpn)):
                    if cnf_rpn[i].isupper():
                        pass
                    elif cnf_rpn[i] == "!":
                        self.assertGreater(i, 0)
                        self.assertTrue(cnf_rpn[i - 1].isupper())
                    else:
                        self.assertIn(cnf_rpn[i], "&|")

                rpn_proc = subprocess.run(["./ex04/print_truth_table", rpn], stdout=subprocess.PIPE, text=True)
                cnf_proc = subprocess.run(["./ex04/print_truth_table", cnf_rpn], stdout=subprocess.PIPE, text=True)

                self.assertEqual(rpn_proc.stdout, cnf_proc.stdout)

class TextEx10(unittest.TestCase):
    def setUp(self):
        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex10/map", "ex10/map.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

        proc = subprocess.run([
            "g++", "-Wall", "-Wextra", "-Werror", "-std=c++11", "-O2",
            "-o", "ex11/reverse_map", "ex11/reverse_map.cpp"
        ])
        if proc.returncode != 0:
            self.skipTest('Compilation failed')

    def tearDown(self) -> None:
        proc = subprocess.run([
            "rm", "-f", "ex10/map"
        ])
        proc = subprocess.run([
            "rm", "-f", "ex11/reverse_map"
        ])

    def test_map_00_noconflict(self):
        result_list = []
        input_list = []

        random.seed(14)

        for _ in range(1000):
            x = random.randint(0, 65535)
            y = random.randint(0, 65535)

            with self.subTest(x=x, y=y):
                map_proc = subprocess.run(["./ex10/map", str(x), str(y)], stdout=subprocess.PIPE, text=True)

                self.assertEqual(map_proc.returncode, 0)

                if (x, y) not in input_list:
                    self.assertNotIn(map_proc.stdout, result_list)
                    input_list.append((x, y))
                    result_list.append(map_proc.stdout)

    def test_map_01_bijective(self):
        random.seed(15)

        for _ in range(1000):
            x = random.randint(0, 65535)
            y = random.randint(0, 65535)

            with self.subTest(x=x, y=y):
                map_proc = subprocess.run(["./ex10/map", str(x), str(y)], stdout=subprocess.PIPE, text=True)

                self.assertEqual(map_proc.returncode, 0)

                revmap_proc = subprocess.run(["./ex11/reverse_map", map_proc.stdout.strip()], stdout=subprocess.PIPE, text=True)

                revcoords = revmap_proc.stdout.split()

                self.assertEqual(len(revcoords), 2)
                self.assertEqual(str(x), revcoords[0])
                self.assertEqual(str(y), revcoords[1])

if __name__ == '__main__':
    unittest.main()
