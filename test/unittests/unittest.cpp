/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* fermin at tid dot es
*
* Author: Ken Zangelin
*/
#include "unittest.h"
#include "testInit.h"



/* ****************************************************************************
*
* debugging - 
*
* FIXME P4 - these counters are useful (only visible if UT_DEBUG is defined)
*            to detect calls to utInit without respective call to utExit.
*            All of it should be removed as soon as we implement the new
*            more complete TRST macro (I_TEST?)
* 
*/
//#define UT_DEBUG
#ifdef UT_DEBUG
static int noOfInits = 0;
static int noOfExits = 0;
#endif



/* ****************************************************************************
*
* Forward declarations
*/
static NotifierMock* notifierMock = NULL;
static TimerMock*    timerMock    = NULL;





/* ****************************************************************************
*
* utInit - unit test init
*
*/
void utInit(void)
{
#ifdef UT_DEBUG
  ++noOfInits;
  printf("**************** IN utInit (%d inits, %d exits)\n", noOfInits, noOfExits);
#endif

  notifierMock = new NotifierMock();
  if (notifierMock == NULL)
  {
    fprintf(stderr, "error allocating NotifierMock: %s\n", strerror(errno));
    exit(1);
  }
  setNotifier(notifierMock);
  
  timerMock = new TimerMock();
  if (timerMock == NULL)
  {
    fprintf(stderr, "error allocating TimerMock: %s\n", strerror(errno));
    exit(1);
  }
  ON_CALL(*timerMock, getCurrentTime()).WillByDefault(Return(1360232700));
  setTimer(timerMock);

  setupDatabase();

#ifdef UT_DEBUG
  printf("**************** FROM utInit (%d inits, %d exits)\n", noOfInits, noOfExits);
#endif
}



/* ****************************************************************************
*
* utExit - unit test exit
*
*/
void utExit(void)
{
#ifdef UT_DEBUG
  ++noOfExits;
  printf("**************** IN utExit (%d inits, %d exits)\n", noOfInits, noOfExits);
#endif

  if (timerMock)
    delete timerMock;

  if (notifierMock)
    delete notifierMock;

  timerMock    = NULL;
  notifierMock = NULL;

  setTimer(NULL);
  setNotifier(NULL);

#ifdef UT_DEBUG
  printf("**************** FROM utExit (%d inits, %d exits)\n", noOfInits, noOfExits);
#endif
}