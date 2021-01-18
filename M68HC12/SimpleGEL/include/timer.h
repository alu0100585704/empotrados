/*!
 *  \brief     Library for managing timer
 *  \author    Alejandro Pérez Moreno
 *  \author    Álvaro Fernández Rodríguez
 *  \date      06/12/2020
 *  \pre       First initialize the timer.
 *  \warning   Changing the scalate mid-execution can cause undefined behaviour
 *  \copyright GNU Public License.
 *  @file
 */
#ifndef SIMPLEGEL_TIMER_H
#define SIMPLEGEL_TIMER_H

#include <types.h>
#include <sys/param.h>
#include <sys/interrupts.h>
#include <sys/sio.h>
#include <sys/locks.h>

/*! @defgroup timer Timer Management

  Set of functions used to manage and use the timer.

 */
/*@{*/

#define  TCM_FACTOR (3)  /*La potencia de 2 a aplicar al factor*/
#define  TCM_FREQ (M6812_CPU_E_CLOCK/(1 << TCM_FACTOR))
/*Pasa de microsegundos a ticks*/
#define  USG_2_TICKS(us)  ((us) * (TCM_FREQ/1000000L))

/*! Init the Timer fixing de prescale.

    This function must to be called before any other in the library.

    @param prescale prescale factor to use in base clock.
*/
void timer_init(uint8_t prescale);

/*! Schedule a function to be called once.

    This function calls once the function \a f after \a useg microseconds.

    @param useg microsecons before calling \a f
    @param f function to be called
    @see timer_repeat_call
*/
void timer_schedule_call(uint32_t useg, void (*f)(void));

/*! Schedule a function to be called periodically.

    This function calls the function \a f every \a useg microseconds.

    @param useg microsecons between \a f calls
    @param f function to be called
    @see timer_schedule_call
*/
void timer_repeat_call(uint32_t useg, void (*f)(void));

/*! Function to get cycles.

    @return number of cycles from system start
*/
uint32_t timer_get_cycle (void);

/*! Function to get microseconds.

    @return number of microseconds from system start.
*/
uint32_t timer_get_micro (void);

/*! Function to get miliseconds.
    This function just calls to \c timer_get_micro and then converts it.
    @return number of miliseconds from system start
*/
uint32_t timer_get_mili (void);

/*! Function to sleep excution.
    This function stops execution for \p useg microseconds.
    @param useg microseconds to be waited
*/
void timer_sleep(uint32_t useg);

//void __attribute__((interrupt)) vi_tov (void);
//void __attribute__((interrupt)) vi_ioc3 (void);
//void __attribute__((interrupt)) vi_ioc2 (void);

/*@}*/
#endif

