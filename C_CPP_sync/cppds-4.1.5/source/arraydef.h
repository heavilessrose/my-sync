#ifndef ARRAYDEF_H
#define ARRAYDEF_H

// Delete subarrays starting from tail node instead of using
// recursive calls to delete operator from the head node
#if defined(__BORLANDC__) | defined(__TURBOC__)
#define ARRAY_DELETE_FROM_TAIL			0
#else
#define ARRAY_DELETE_FROM_TAIL			1
#endif

// Implement STL compatible array iterators
#define ARRAY_IMPLEMENT_STLITERATOR		1

// Implement STL compatible container methods
#define ARRAY_IMPLEMENT_STLCONTAINER		1

// Method trace debugging code
#define ARRAY_TRACE	0
#if ARRAY_TRACE
#  define ARXMARKER(x, strm)	XMARKER(x, strm)
#else
#  define ARXMARKER(x, strm)
#endif

// Error stream for debugging code
#define ARRAY_ERRSTREAM  cout

#endif




