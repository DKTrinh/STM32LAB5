# Finite State Machine Documentation

## 1. Command Parser FSM
<img width="724" height="543" alt="image" src="https://github.com/user-attachments/assets/8308257b-9b51-4907-806c-b678960d6cd7" />

The Command Parser FSM, depicted on the left side of the figure, manages the reception and validation of incoming data strings. Its operation is defined as follows:

* **INIT State**: The system remains in an idle state, monitoring the incoming data stream for a specific start delimiter.
    * When the system detects the character `'!'`, it transitions to the **READ** state to begin capturing the command.

* **READ State**: This is the active data accumulation state.
    * **Data Buffering**: The system continuously captures incoming characters and saves them to the `cmd_store` buffer.
    * **Termination Check**: The system monitors for the end delimiter `'#'`. If `'#'` is detected **and** the length of the command (`len(cmd)`) is greater than 3, the FSM transitions to the **STOP** state.

* **STOP State**: This state signifies the end of a data packet reception.
    * The system performs a "Check the command" action to validate the integrity or content of the received string.
    * Once validation is complete, the FSM automatically transitions back to the **INIT** state to await the next command sequence.

---

## 2. UART Communication FSM
<img width="885" height="523" alt="image" src="https://github.com/user-attachments/assets/cbe0f8a7-80c3-4809-84b8-8a9bab294dd8" />

The UART Communication FSM, depicted on the right side of the figure, manages the response logic and timeout mechanisms of the protocol. Its operation is defined as follows:

* **INIT State**: The system remains in an idle state, waiting for the Command Parser to flag a valid `RST` command.

* **RST State**: This is the active communication state where the system waits for an acknowledgment (`OK`) from the host.
    * **Timeout Handling**: If the host fails to respond within the designated time (indicated by `Timer Flag == 1`), the FSM performs a retry mechanism. This typically involves re-reading sensors or re-transmitting the data packet before restarting the timer.
    * **Success**: If the Command Parser detects an `OK` command, the FSM transitions to the **OK** state.

* **OK State**: This state signifies the successful completion of the communication session.
    * The system performs necessary cleanup operations (such as clearing cached values) and transitions back to the **INIT** state to prepare for new requests.
