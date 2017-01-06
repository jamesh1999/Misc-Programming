package mobile.lilrocks.com.network;

/**
 * Created by james on 26/10/2016.
 */

import java.io.IOException;
import java.net.Socket;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;

public class NetworkThread implements Runnable
{
	private static NetworkThread instance;

	private String dstIP;
	private int dstPort;

	private Socket socket;
	private DataInputStream inputStream;
	private DataOutputStream outputStream;
	private boolean running = false;
	private boolean toConnect = false;
	private final Object lock;

	//Queues to talk with other threads
	private final Queue<String> inputQueue;
	private final Queue<String> outputQueue;

	private NetworkThread()
	{
		inputQueue = new LinkedList<>();
		outputQueue = new LinkedList<>();

		lock = new Object();
	}

	private void start()
	{
		try
		{
			//Initialize socket
			socket = new Socket(dstIP, dstPort);
			//Bind IO streams
			inputStream = new DataInputStream(socket.getInputStream());
			outputStream = new DataOutputStream(socket.getOutputStream());

			synchronized (lock)
			{
				running = true;
				toConnect = false;
			}
		}
		catch (IOException e)
		{
			System.out.println(e);
		}
	}

	private void stop()
	{
		try
		{
			outputStream.writeBytes("bye\r\n");

			if(socket != null)
				socket.close();

			if(inputStream != null)
				inputStream.close();

			if(outputStream != null)
				outputStream.close();

			synchronized (lock)
			{
				running = false;
				toConnect = false;
			}
		}
		catch(IOException e)
		{
			System.out.println(e);
		}
	}

	public final void run()
	{
		//Networking loop
		while(!Thread.currentThread().isInterrupted())
		{
			synchronized (lock)
			{
				//Handle server connect
				if (!running)
				{
					if(toConnect)
						start();
					continue;
				}

				//Handle server disconnect
				if(toConnect)
				{
					stop();
					continue;
				}

				//Handle messages
				try
				{
					//Read from socket
					synchronized (inputQueue)
					{
						String message = readMessageInternal();
						while (message.length() > 0)
						{
							//Reply to heartbeat packets
							if (message.equals("Heartbeat"))
								writeMessageInternal("HeartbeatReturn");
								//Add message to queue
							else
								inputQueue.add(message);

							message = readMessageInternal(); //Read next message
						}
					}

					//Write to socket
					synchronized (outputQueue)
					{
						while (!outputQueue.isEmpty())
							writeMessageInternal(outputQueue.remove());
					}
				} catch (IOException e)
				{
					System.out.println(e);
				}
			}
		}
	}

	private char[] message = new char[1024];
	private String readMessageInternal() throws IOException
	{
		byte buff;
		boolean seenCR = false;
		int index = 0;
		while(inputStream.available() > 0)
		{
			buff = inputStream.readByte();

			//End of message
			if(seenCR && buff == '\n')
				break;

			if(buff == '\r')
				seenCR = true;
			else
			{
				message[index++] = (char) buff;
				seenCR = false;
			}
		}

		return new String(Arrays.copyOfRange(message, 0, index));
	}

	private void writeMessageInternal(String message) throws IOException
	{
		outputStream.writeBytes(message);
		outputStream.writeBytes("\r\n");
	}

	public static NetworkThread getInstance()
	{
		if(instance == null)
			instance = new NetworkThread();
		return instance;
	}

	public static void connect(String ip, int port)
	{
		instance.dstIP = ip;
		instance.dstPort = port;

		synchronized (instance.lock)
		{
			instance.toConnect = true;
		}
	}

	public static void disconnect()
	{
		synchronized (instance.lock)
		{
			instance.toConnect = true;
		}
	}

	public static String readMessage()
	{
		synchronized (instance.inputQueue)
		{
			return instance.inputQueue.isEmpty() ? "" : instance.inputQueue.remove();
		}
	}

	public static void writeMessage(String message)
	{
		synchronized (instance.outputQueue)
		{
			instance.outputQueue.add(message);
		}
	}
}