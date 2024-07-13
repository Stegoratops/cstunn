#ifndef CSTUNN_CONFIG_H_INCLUDED
#define CSTUNN_CONFIG_H_INCLUDED

/* 
 * Describes what should happen if there is an attempt to write a value
 * from an array at an index larger than the size of the array
 * 
 * Should be a value from any of the CSTUNN_CV_ON_OUT_OF_BOUNDS_* macros
 */
#define CSTUNN_C_ON_OUT_OF_BOUNDS CSTUNN_CV_ON_OUT_OF_BOUNDS_THROW

/* 
 * Describes what should happen if there is an attempt to write the implicit
 * NUL-Char of a string-literal beyond the boundries of the current array
 * 
 * Should be a value from any of the CSTUNN_CV_ON_OUT_OF_BOUNDS_* macros
 * 
 * Default is CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP as this matches the behaviour of
 * the typical C-compiler
 */
#define CSTUNN_C_ON_NULCHAR_OUT_OF_BOUNDS CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP

/* The parser stops, returning an error code to the caller */
#define CSTUNN_CV_ON_OUT_OF_BOUNDS_THROW 0
/* 
 * The parser continues as nothing happened,
 * without writing any of the OOB-values
 * to the output-buffer
 */
#define CSTUNN_CV_ON_OUT_OF_BOUNDS_NOOP  1
/*
 * The parser continues as nothing happened,
 * writing all values of EVEN THE OOB-VALUES!!
 * to the output-buffer
 * 
 * THIS OPTION CAUSES NOT ONLY TO OVERRUN THE BUFFER
 * OF THE CURRENT ARRAY, BUT MAY ALSO WRITE BEYOND
 * THE SUPPLIED BUFFER IN GENERAL
 */
#define CSTUNN_CV_ON_OUT_OF_BOUNDS_WRITE 2
/*
 * Stops the ENTIRE programm
 */
#define CSTUNN_CV_ON_OUT_OF_BOUNDS_TRAP  3

#endif /* CSTUNN_CONFIG_H_INCLUDED */
