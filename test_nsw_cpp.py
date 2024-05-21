# -*-coding:utf-8 -*-
"""
:创建时间: 2024/5/21 22:53
:作者: 苍之幻灵
:我的主页: https://cpcgskill.com
:Github: https://github.com/cpcgskill
:QQ: 2921251087
:aboutcg: https://www.aboutcg.org/teacher/54335
:bilibili: https://space.bilibili.com/351598127
:爱发电: https://afdian.net/@Phantom_of_the_Cang

"""

from __future__ import unicode_literals, print_function, division

if False:
    from typing import *
import subprocess
import matplotlib.pyplot as plt


def run_test():
    subprocess.run(['g++', '-std=c++17', '-O3', '-o', 'test', 'nsw.cpp', 'test.cpp'])
    subprocess.run(['./test.exe'])


def read_data(filename):
    x = []
    y = []
    with open(filename, 'r') as f:
        for line in f:
            n, time = map(float, line.split())
            x.append(n)
            y.append(time)
    return x, y


def plot_results():
    init_x, init_y = read_data('init_times.txt')
    query_x, query_y = read_data('query_times.txt')

    plt.figure(figsize=(10, 4))

    plt.subplot(1, 2, 1)
    plt.plot(init_x, init_y, label='Initialization Time')
    plt.xlabel('Number of Nodes (n)')
    plt.ylabel('Time (seconds)')
    plt.title('Performance of Initialization')

    plt.subplot(1, 2, 2)
    plt.plot(query_x, query_y, label='Average Query Time', color='orange')
    plt.xlabel('Number of Nodes (n)')
    plt.ylabel('Time (seconds)')
    plt.title('Performance of find_nearest')

    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    run_test()
    plot_results()
