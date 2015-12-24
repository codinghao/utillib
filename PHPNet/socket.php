<?php

define("EVENT_NONE",   0);
define("EVENT_TIMER",  1);
define("EVENT_SEND",   2);
define("EVENT_RECV",   4);
define("EVENT_CLOSE",  8);
define("EAGAIN", 11);
define("READ_BUFFER_SIZE", 4086);

class socket
{
	private $handle = null;
    private $buffer = '';

	public function connect($ip, $port)
	{
		$this->handle = @socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($this->handle === false)
        	return false;

		$ret = @socket_connect($this->handle, $ip, $port);
		if ($ret === false)
        {
            $this->close();
			return false;
        }

		@socket_set_nonblock($this->handle);

		return true;
	}

	public function send($buffer)
	{
		if ($this->handle == null)
			return false;

        $this->buffer = $this->buffer.$buffer;
        $leftLen = strlen($this->buffer);
        
		while($leftLen > 0)
		{
			$ret = @socket_write($this->handle, $this->buffer, $leftLen);
			if ($ret === false)
            {
                if ($this->error() == EAGAIN)
                    continue;

				return false;
            }

			$leftLen -= $ret;
            $this->buffer = $leftLen > 0 ? substr($this->buffer, -$this->leftLen) : '';
		}

		return true;
	}

	public function recv($len)
	{
		if ($this->handle == null)
			return false;

		$ret = @socket_read($this->handle, $len);
		if ($ret === false || $ret == "")
			return false;

		return $ret;
	}

	public function select($timeOut)
	{
		$read = array($this->handle);
		$write = array($this->handle);
		$write = null;
		$except = null;
		
		$ret = @socket_select($read, $write, $except, $timeOut, $timeOut);
		if ($ret === false)
			return false;

	    $event = EVENT_NONE;	
		if ($ret > 0 && !empty($read))
			$event = $event | EVENT_RECV;

		if ($ret > 0 && !empty($write))
			$event = $event | EVENT_SEND;

        return $event;
	}

    public function error()
    {
        return @socket_last_error($this->handle);
    }

	public function close()
	{
		if ($this->handle != null)
			@socket_close($this->handle);

		$this->handle = null;
	}
}
?>
