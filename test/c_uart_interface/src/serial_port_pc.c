#include <inc/serial_port.h>

mavlink_status_t status;
uint8_t msgReceived = false;

extern volatile float seconds;

//extern struct termios oldtio,newtio;
struct termios oldtio, newtio;
static int fd;
const char* RS232_DEVICE_const;

// Initialisation
void serial_start(const char* portname)
{
	RS232_DEVICE_const = portname;
	printf("Open port : %s\n", portname);
	fd = open(portname, O_RDWR | O_NOCTTY);

	if (fd < 0){
		perror(portname);
		exit(-1);
	}

	tcgetattr(fd, &oldtio);							  /* save current serial port settings */
	bzero(&newtio, sizeof(newtio));					  /* clear struct for new port settings */
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD; /* _no_ CRTSCTS */
	newtio.c_iflag = IGNPAR;						  // | ICRNL |IXON;
	newtio.c_oflag = IGNPAR;						  //ONOCR|ONLRET|OLCUC;
	newtio.c_cc[VTIME] = 0;							  /* inter-character timer unused */
	newtio.c_cc[VMIN] = 1;							  /* blocking read until 1 character arrives */
	printf("Succeeded\n");
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
}


// Serial Read
int serial_read_message(mavlink_message_t* message)
{
	msgReceived = mavlink_parse_char(MAVLINK_COMM_1, usart_recv_blocking(USART3), message, &status);
	return msgReceived;
}


int usart_recv_blocking(int i)
{
	char c;
	read(fd, &c, 1);
	return (c);
}


// Serial write
int serial_write_message(const mavlink_message_t* message)
{
	char buff[300];
	int bytesWritten;
	unsigned len = mavlink_msg_to_send_buffer((uint8_t *)buff, message);

	for (unsigned short i = 0; i < sizeof(buff); i++)	{
		write(fd, &buff[i], 1);
	}

	bytesWritten = len;
	len = 0;
	return bytesWritten;
}

// gettimeofday

int get_time_sec(struct timeval *tv, struct timezone *tz)
{
	return gettimeofday(tv, tz);
}