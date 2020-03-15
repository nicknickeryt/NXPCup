#include <string.h>
#include "dualcore_common.h"
#include "ipc_msg.h"

/*
 * IRQ priority of IPC interrupt
 * Currently this has the highest priority
 */
#define IPC_IRQ_Priority    IRQ_PRIO_IPC

#ifdef CORE_M0
static struct ipc_queue *qrd = (struct ipc_queue *)SHARED_MEM_M4;
static struct ipc_queue *qwr = (struct ipc_queue *)SHARED_MEM_M0;
#define IPC_IRQHandler M4_IRQHandler
#define ClearTXEvent   Chip_CREG_ClearM4Event
#define IPC_IRQn       M4_IRQn
#else
#error "For LPC43XX, CORE_M0 must be defined!"
#endif

/*****************************************************************************
 * uCOS-III functions
 ****************************************************************************/

/* Wait for the event */
#define ipc_wait_event(evt, sem) while ((evt))

#define ipc_wait_event_tout(evt, tout, sem)	\
	do { \
		uint32_t cnt = Chip_RIT_GetCounter(LPC_RITIMER) + (tout * (SystemCoreClock / 1000)); \
		if (cnt + 5000 < cnt) {cnt += 5000; } \
		while ((evt) && Chip_RIT_GetCounter(LPC_RITIMER) < cnt) {}	 \
		if (evt) {tout = 0; } \
	} while (0)

static void ipc_misc_init(void)
{}



/*
 * Initiate interrupt on other processor
 * Upon calling this function generates and interrupt on the other
 * core. Ex. if called from M0 core it generates interrupt on M4 core
 * and vice versa.
 */
static void ipc_send_signal(void)
{
	__DSB();
	__SEV();
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Function to send notificaton interrupt */
void IPC_msgNotify(void)
{
	ipc_send_signal();
}

/* Function to initialize the IPC message queue */
void IPC_initMsgQueue(void *data, int size, int count)
{
	/* Sanity Check */
	if (!size || !count || !data) {
		DEBUGSTR("ERROR:IPC Queue size invalid parameters\r\n");
		while (1) {}
	}

	/* Check if size is a power of 2 */
	if (count & (count - 1)) {
		DEBUGSTR("ERROR:IPC Queue size not power of 2\r\n");
		while (1) {	/* BUG: Size must always be power of 2 */
		}
	}

	memset(qwr, 0, sizeof(*qwr));
	ipc_misc_init();
	qwr->count = count;
	qwr->size = size;
	qwr->data = data;
	qwr->valid = QUEUE_MAGIC_VALID;
	NVIC_SetPriority(IPC_IRQn, IPC_IRQ_Priority);
	NVIC_EnableIRQ(IPC_IRQn);
}

/* Function to push a message into queue with timeout */
int IPC_pushMsgTout(const void *data, int tout)
{
	/* Check if write queue is initialized */
	if (!QUEUE_IS_VALID(qwr)) {
		return QUEUE_ERROR;
	}

	if (tout == 0) {
		/* Check if queue is full */
		if (QUEUE_IS_FULL(qwr)) {
			return QUEUE_FULL;
		}
	}
	else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event(QUEUE_IS_FULL(qwr), event_tx);
	}
	else {
		/* Wait for read queue to have some data */
		ipc_wait_event_tout(QUEUE_IS_FULL(qwr), tout, event_tx);
		if (tout == 0) {
			return QUEUE_TIMEOUT;
		}
	}

	memcpy(qwr->data + ((qwr->head & (qwr->count - 1)) * qwr->size), data, qwr->size);
	qwr->head++;
	ipc_send_signal();

	return QUEUE_INSERT;
}

/* Function to read a message from queue with timeout */
int IPC_popMsgTout(void *data, int tout)
{
#ifdef EVENT_ON_RX
	int raise_event = QUEUE_IS_FULL(qrd);
#endif

	if (!QUEUE_IS_VALID(qrd)) {
		return QUEUE_ERROR;
	}

	if (tout == 0) {
		/* Check if read queue is empty */
		if (QUEUE_IS_EMPTY(qrd)) {
			return QUEUE_EMPTY;
		}
	}
	else if (tout < 0) {
		/* Wait for read queue to have some data */
		ipc_wait_event(QUEUE_IS_EMPTY(qrd), event_rx);
	}
	else {
		/* Wait for event or timeout */
		ipc_wait_event_tout(QUEUE_IS_EMPTY(qrd), tout, event_rx);
		if (tout == 0) {
			return QUEUE_TIMEOUT;
		}
	}

	/* Pop the queue Item */
	memcpy(data, qrd->data + ((qrd->tail & (qrd->count - 1)) * qrd->size), qrd->size);
	qrd->tail++;

#ifdef EVENT_ON_RX
	if (raise_event) {
		ipc_send_signal();
	}
#endif
	return QUEUE_VALID;
}

/* Get number of pending items in queue */
int IPC_msgPending(int queue_write)
{
	struct ipc_queue *q = queue_write ? qwr : qrd;
	if (!QUEUE_IS_VALID(q))
		return QUEUE_ERROR;

	return QUEUE_DATA_COUNT(q);
}

/**
 * @brief	Call-back function to handle IPC Message receive event
 * @return	None
 * This is a weak function (hence can be overridden by the user's application
 * with a function of same name. This call-back function will be called
 * whenever there is a message received by the core that implements this
 * function.
 */
#ifdef __IAR_SYSTEMS_ICC__
__weak void EVENT_IPC_Receive(void)
#else
void EVENT_IPC_Receive(void) __attribute__ ((weak));

void EVENT_IPC_Receive(void)
#endif
{}

/**
 * @brief	Interrupt handler for IPC interrupts
 * @return	None
 */
void IPC_IRQHandler(void)
{
	/* Clear the interrupt */
	ClearTXEvent();

	/* Invoke handler */
	EVENT_IPC_Receive();
}

/* Function to convert IPC error number to string */
const char *IPC_strerror(int errnum)
{
	static const char *ipc_errstr[] = {
		"Queue Insert OK",
		"Queue Pop OK/Valid",
		"Queue is Full",
		"Queue is Empty",
		"Queue Error/Not initialized",
		"Queue operation timed out",
	};
	if (errnum < 0) {
		errnum = 1 - errnum;
	}
	return ipc_errstr[errnum];
}

/**
 * @}
 */






