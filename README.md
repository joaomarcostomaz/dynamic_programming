This C++ program aims to find the maximum achievable score for a given set of sections on a track, each associated with a bonus and crossing time, and a set of maneuvers, each associated with a score and duration. The goal is to determine the optimal sequence of maneuvers for each section to maximize the overall score. This problem is based on the Knapsack problem.

Each input consists of several lines. The first line contains two integers, N and K, representing, respectively, the number of maneuver sections on the track and the number of maneuvers cataloged in Radlandia; it is guaranteed that 1 ≤ N ≤ 100 and 1 ≤ K ≤ 10. Each of the following N lines describes a section of the track. The i-th of these lines contains two integers: ci, representing the bonus factor of the section (1 ≤ ci ≤ 100), and li, representing the traversal time of the section (1 ≤ li ≤ 10^6). Then, there are K lines, each describing a maneuver. The j-th of these lines contains two integers: the base score pj (−10^6 ≤ pj ≤ 10^6) of the maneuver and the time tj required to execute the maneuver (1 ≤ tj ≤ 10^6); assume that the maneuvers are numbered from 1 to K in the order given in the input.

The output contains multiple lines. The first line should print a single integer T, representing the maximum total score that Diná can achieve. Then, N lines follow, each with several integers. The i-th of these lines represents the i-th maneuver section. The first integer ni in this line represents the number of maneuvers to be performed in the section; then, ni numbers should be printed, each representing a maneuver performed in the i-th section.