# Computer-Vision
#grahamscan.cpp
An algorithm to find the convex hull of a set of points
#canny.cpp
A canny edge detection algorithm which finds edges in an input image and utilizes a guassian filter and intensity of the gradient to detect where an edge is. 
#hough.cpp
A Hough transformation algorithm which detects and draws circles when objects meet a certain threshold of looking like circles. The process works with through a 
voting process where points detected from the canny edge detection algorithm vote for a center point and a radius, and the point with the most votes becomes the 
center and we draw a circle around that point with the most voted radius associated with that point. This allows us to detect where circles lie in our images.
