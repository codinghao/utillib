<?php

require_once('socket.php');

class timer
{
	private $timeOut = 0;
	private $endTime = 0;
	private $handle = null;

	function __construct($timeOut, $handle)
	{
		$this->timeOut = $timeOut;
		$this->endTime = time() + $timeOut;
		$this->handle  = $handle;
	}

	public function sleep_time()
	{
		$time = time();
		return $time > $this->endTime ? 0 : ($this->endTime - $time);
	}

	public function timer_run()
	{
		$time = time();
		if ($time > $this->endTime)
		{
			call_user_func($this->handle);
			$this->endTime = time() + $this->timeOut;
			return $this->timeOut;
		}

		return $this->endTime - $time;
	}
}

class session
{
	private $socket              = null;
	private $buffer              = "";
	private $timer               = null;
	private $readHandle          = null;
	private $writeHandle         = null;
	private $connectSuccHandle   = null;
	private $connectFaildHandle  = null;
	private $connectBrokenHandle = null;
	private $emptyHandle         = null;

	function __construct()
	{
		$this->emptyHandle         = function($arg1 = null, $arg2 = null, $arg3 = null) {};
		$this->readHandle          = $this->emptyHandle;
		$this->writeHandle         = $this->emptyHandle;
		$this->connectSuccHandle   = $this->emptyHandle;
		$this->connectFaildHandle  = $this->emptyHandle;
		$this->connectBrokenHandle = $this->emptyHandle;
	}
	
	public function connect($ip, $port)
	{
		$this->socket = new socket();
		if ($this->socket->connect($ip, $port) === false)
		{
			call_user_func($this->connectFaildHandle, $this);
			return false;
		}

		call_user_func($this->connectSuccHandle, $this);

		return true;
	}

	public function send($data)
	{
		return $this->socket->send($data);
	}
	
	public function core_run()
	{
		$timeOut = $this->timer != null ? $this->timer->sleep_time() : 5;
        $retVal = $this->core_process($timeOut);
		if ($retVal === false)
			return false;
        if ($retVal === EVENT_CLOSE)
        {
            $this->socket->close();
            call_user_func($this->connectBrokenHandle, $this);
        }
		
        if ($this->timer != null)
			$this->timer->timer_run();

		return true;
	}

	public function bind_read($handle)
	{
		$this->readHandle = empty($handle) ? $this->emptyHandle : $handle;
	}

	public function bind_write($handle)
	{
		$this->writeHandle = empty($handle) ? $this->emptyHandle : $handle;
	}

	public function bind_connect_succ($handle)
	{
		$this->connectSuccHandle = empty($handle) ? $this->emptyHandle : $handle;
	}

	public function bind_connect_faild($handle)
	{
		$this->connectFaildHandle = empty($handle) ? $this->emptyHandle : $handle;
	}

	public function bind_connect_broken($handle)
	{
		$this->connectBrokenHandle = empty($handle) ? $this->emptyHandle : $handle;
	}

	public function bind_timer($timeOut, $handle)
	{
		$this->timer = new timer($timeOut, $handle);
	}

	private function core_process($timeOut)
	{
		$retVal = $this->socket->select($timeOut);
		if ($retVal === false)
			return false;
        
        if ($retVal & EVENT_RECV)
				$retVal = $this->recv_process();
        if ($retVal & EVENT_SEND)
				$retVal &= $this->send_process();

        return $retVal === false ? EVENT_CLOSE : true;
	}

	private function send_process()
	{
		call_user_func($this->writeHandle, $this);
        return $this->socket->send('', 0);
	}

	private function recv_process()
	{
		while (true) 
		{
			$ret = $this->socket->recv(READ_BUFFER_SIZE);
			if ($ret === false)
            {
                if ($this->socket->error() == EAGAIN)
                        continue; 

				return false;
            }

            $this->buffer = $this->buffer.$ret;

            if ($ret < READ_BUFFER_SIZE)
            	break;
		}

		$useLen = call_user_func($this->readHandle, $this, $this->buffer);
		$this->buffer = $useLen >= strlen($this->buffer) ? "" : substr($this->buffer, $useLen);

		return true;
	}
}
?>
