import matplotlib.pyplot as plt
from monitor import *
import numpy as np

DATA_FOLDER = "./data"

def read_report(path:str):
    rep = open(path, "r")
    lines = rep.readlines()
    times = []
    for idx, line in enumerate(lines):
        if idx < NUM_OF_TRIALS:
            times.append(float(line.strip()))
            continue
        
        d = line.strip().split(":")
        
    rep.close()


def build_stress_test_bar_chart():

