================================================================================
OZCollide v1.0.0                                                    May 15, 2006

Copyright (C) 2006 by Igor Kravtchenko
================================================================================
http://www.tsarevitch.org/ozcollide/
contact the author: igor@tsarevitch.org

Ã÷¬€:
http://www.dingoogames.cn/bbs/read.php?tid=31

================================================================================


What is OZCollide?

OZCollide is a fast, complete and free collision detection library. It provides a very ease of use C++ API especially focused on performance and functionalities for real time applications such as video games. OZCollide doesn't just provide yes/no collision queries between standard primitives. It is a full, functional, performant, stable and cross-platform library to be used instantly into your pipeline production providing tons of collision methods up to the object serialization itself. OZCollide is based on AABB tree implementation. See the next section for more informations.

What are the features of OZCollide?

- Supports for polygon soup, not just triangles as other libraries but real n-gons soup allowing performant indexation of convex and concave geometry
- Different structures to best match the nature of collisionable entities: n-gons, AABBs and spheres
- Large number of native API primitives to provide all kind of standard queries: sphere, ellipsoid, AABB, OBB, triangle, point, line and frustum
- Swept collision detection to detect the exact moment of collision of movable primitives (LSS) whatever their speed
- Supports for static collision detection on a given time as well
- Query for simple collision (true/false) or complete list of intersections
- Triangle/AABB, Triangle/OBB, Triangle/Sphere collision and intersection detection
- Line/Line, Line/Box, Segment/Triangle intersection detection
- Sphere/Box, Sphere/Line, Sphere/Frustum, AABB/OBB, ...
- ... really tons of methods for all your needs
- Radix based "Sweep'n'prune" for large number of movable entities
- System of cache with temporal coherence to greatly speed up access of collision resolution
- User callback to "take hand" when a collision occurs
- User value on leaf to hold any personnal data
- Built-in AABB trees builder to automatically build collision trees and monitor the result
- Native IO serializer (up to you to use it or no) based on IFF (Interchange File Format) with 'reading-from-memory' capabilities
- Cross-platform, works on Windows, Linux and Macintosh, in fact any platform supporting VisualStudio 6/7 or gcc
- Native IO data access Class to not care about FPU internal format (IEEE754 compliant or not), endianness or anything
- Support for PCH (Precompiled Header) or not
- Highly optimised code and algorithms
- Can be used as a static or dynamic library
- Released under the LGPL license
- Ease to use, documented, stable and very performant
- ...and much more!

I am not very familiar with collision/intersection algorithms. I just want to implement some kind of collision for say, my particles system. Is that would be hard with OZCollide?

No. Actually it is a matter of maybe 1 hour or 2 if you do not know yet the API.

No words. How is it working in practice?

You give to OZCollide your geometry defined by a set of points and a set of polygons. The library build for you a tree and return a pointer to it. Your particles are points that define lines when moving. Just invoke a query on that tree with these lines and you obtain your moments of collision if any. It is a matter of some lines of code and lighting fast.

What about a caracter player moving in a 3d world?

Same thing except here you would prefer to represent your caracter by a box, a sphere or a capsule to better fit its shape. Note that if you decide to use the swept detection algorithm you do not have even to worry about the velocity of your movable entities. Collision will always be detected even on a low machine with a limited number of FPS.

How fast is OZCollide?

OZCollide has been already used successfully to solve collision detection of hundreds of movable entities inside world geometry represented by tens of thousands of triangles and it never exceeded 5% of total CPU on a PIII-700Mhz. Also, OZCollide is "memory-friendly" and avoid allocation as much as possible prefering large chunk of memory at once (that is also limits memory fragmentation) which is especially appreciable for consoles development.

Can I freely use OZCollide in my personnal/commercial/whatever program?

Absolutely. OZCollide is released under the LGPL license. However a kind greetings is always appreciated :)

Where can I download OZCollide?

Right here: ozcollidelib_1-0-0.zip

Any documentation?

There is an online documentation available here.

Question?

Don't hesitate to write me for any question, suggestion or feedback:
igor at tsarevitch.org
