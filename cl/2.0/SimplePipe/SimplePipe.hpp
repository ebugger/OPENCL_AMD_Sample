/**********************************************************************
Copyright �2015 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

�   Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
�   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/
#ifndef SIMPLE_PIPE_H_
#define SIMPLE_PIPE_H_

#define NUM_OF_PACKETS  (16384)
#define PACKET_SIZE 4
#define NUM_PIPES 4

#define SAMPLE_VERSION "AMD-APP-SDK-v3.0.130.1"

//Header Files
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "CLUtil.hpp"
#include <CL/cl.h>

using namespace appsdk;

/**
 * SimplePipe
 * Class implements OpenCL SimplePipe Pipe-Intro sample
 */

class SimplePipe
{
		double  setupTime;			   /**< Time for setting up OpenCL */
        double  seqTime;			   /**< Sequential kernel run time */
        double  conTime;		       /**< Concurrent kernel run time */

        cl_uint *input;                /**< Input array */
	cl_uint *output;			   /**< Output array */
        cl_uint refOut;                /**< Reference output */
	cl_uint actOut;				   /**< Actual output */

        cl_context context;            /**< CL context */
        cl_device_id *devices;         /**< CL device list */
	cl_command_queue commandQueue; /**< CL command queue */
        cl_program program;            /**< CL program  */
	cl_kernel writePipeKernel;     /**< CL producer kernel */
	cl_kernel readPipeKernel;      /**< CL consumer kernel */
	size_t localWorkItems;		   /**< Work group size */
	size_t globalWorkItems;		   /**< Global threads  */

        cl_mem inputBuffer;            /**< CL input memory buffer */
	cl_mem outputBuffer;           /**< CL output memory buffer */
	cl_mem pipe[NUM_PIPES];        /**< CL pipe memory buffer, pipe objects are not accessble from host*/

	cl_int  isMultiPipe;		   /**< A Flag variable, indicating singlePipe or multiPipe use cases */
	cl_int numPipes;		   /**< Number of pipes */
	int  kernelType;		   /**< Type of kernel, to specify which built-in Pipe functions should be used */
	int  numComputeUnits;          /**< Number of compute units in the GPU */
    int  iterations;               /**< Number of iterations for kernel execution*/
    int  numWGsPerKernel;           /**< Number of WorkGroups per kernel run */
    cl_uint localSize;
	cl_uint numPackets;            /**< Total Number of packets */
	cl_uint numPacketsPerPipe;	   /**< Number of packets Per Pipe*/
	cl_uint packetSize;            /**< Packet size in Bytes */


        SDKDeviceInfo deviceInfo;      /**< Structure to store device information*/
        KernelWorkGroupInfo kernelInfoC,
                            kernelInfoG;   
							           /**< Structure to store kernel related info */
        SDKTimer    *sampleTimer;      /**< SDKTimer object */
		int               Timer;    /**< Timer */
    public:

        CLCommandArgs   *sampleArgs;   /**< CLCommand argument class */

        /**
         * Constructor
         * Initialize member variables
         */
        SimplePipe()
            :input(NULL),
			 output(NULL),
			 iterations(1),
			 inputBuffer(NULL),
             outputBuffer(NULL),
             refOut(0),
			 actOut(0),
             devices(NULL),
			 commandQueue(NULL),
			 isMultiPipe(0),
			 numPipes(1),
			 kernelType(0),
             numWGsPerKernel(10),
             localSize(64),
			 numPackets(NUM_OF_PACKETS),
			 numPacketsPerPipe(NUM_OF_PACKETS),
			 packetSize(PACKET_SIZE),
			 setupTime(0),
			 seqTime(0),
			 conTime(0)
        {
            sampleArgs = new CLCommandArgs() ;
            sampleTimer = new SDKTimer();
            sampleArgs->sampleVerStr = SAMPLE_VERSION;
        }

        /**
         * Allocate and initialize host memory array with random values
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int setupSimplePipe();

        /**
         * OpenCL related initialisations.
         * Set up Context, Device list, Command Queue, Memory buffers
         * Build CL kernel program executable
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int setupCL();

        /**
         * Override from SDKSample. Initialize
         * command line parser, add custom options
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int initialize();

        /**
         * Override from SDKSample, Generate binary image of given kernel
         * and exit application
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int genBinaryImage();

        /**
         * Override from SDKSample, adjust width and height
         * of execution domain, perform all sample set-up
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int setup();

        /**
         * Override from SDKSample
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int run();

        /**
         * Override from SDKSample
         * Clean-up memory allocations
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int cleanup();

        /**
         * Override from SDKSample
         * Verify against reference implementation
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int verifyResults();


        /**
         * Prints data and performance results
         */
        void printStats();


        /**
         * Execute kernels sequentially in a single queue
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int runKernels();

        /**
         * Reference implementation to find
         * the occurrences of a value in a given array
         */
        void cpuRefImplementation();

		/**
         * Get the size of largest OpenCL built-in data type given OpenCl devices
		 * @param minAlignment a pointer to an integer variable to store size of largest OpenCL built-in datatype (in bits)
         * @return SDK_SUCCESS on success and SDK_FAILURE on failure
         */
        int getMinAlignment(int* minAlignment);
};
#endif // SIMPLE_PIPE_H_
