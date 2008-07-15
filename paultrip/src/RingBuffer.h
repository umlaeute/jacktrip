//*****************************************************************
/*
  PaulTrip: A System for High-Quality Audio Network Performance
  over the Internet

  Copyright (c) 2008 Juan-Pablo Caceres, Chris Chafe.
  SoundWIRE group at CCRMA, Stanford University.
  
  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the "Software"), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:
  
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/
//*****************************************************************

/**
 * \file RingBuffer.h
 * \author Juan-Pablo Caceres
 * \date July 2008
 */

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

#include "types.h"

/** \brief Provides a ring-buffer (or circular-buffer) that can be written to and read from
 * asynchronously (blocking).
 *
 * The RingBuffer is an array of \b NumSlots slots of memory
 * each of which is of size \b SlotSize bytes (8-bits). Slots can be read and 
 * written asynchronously by multiple threads.
 */
class RingBuffer
{
public:

  /** \brief The class constructor
   * \param SlotSize Size of one slot in byes
   * \param NumSlots Number of slots
   */
  RingBuffer(int SlotSize, int NumSlots);

  /** \brief The class destructor
   */
  virtual ~RingBuffer();
  
  /** \brief Write a chunk into the RingBuffer
   *
   * The caller is responsible to make sure sizeof(writeSlot) = slotSize
   */
  void writeSlot(const int8_t* WriteSlot);
  
  /** \brief Read a slot from the RingBuffer
   */
  void readSlot(int8_t* ReadSlot);

  /** \brief Read the last available slot, without blocking if there are no new 
   * slots
   */
  void readLastSlotNonBlocking(int8_t* ReadSlot);

private:

  void underrunReset();
  void overflowReset();

  int8_t* mRingBuffer; ///< 8-bit array of data (1-byte)
  int8_t* mLastReadSlot; ///< Last slot read  
  const int mSlotSize; ///< The size of one slot in byes
  const int mNumSlots; ///< Number of Slots
  int mTotalSize; ///< Total size of the mRingBuffer = mSlotSize*mNumSlotss
  int mReadPosition; ///> Read Positions in the RingBuffer (Tail)
  int mWritePosition; ///> Write Position in the RingBuffer (Head)
  int mFullSlots; ///> Number of used (full) slots, in slot-size

  //Thread Synchronization Private Members
  QMutex mMutex; ///> Mutex to protect read and write operations
  QWaitCondition mBufferIsNotFull; ///> Buffer not full condition to monitor threads
  QWaitCondition mBufferIsNotEmpty; ///> Buffer not empty condition to monitor threads
};

#endif
