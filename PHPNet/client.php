<?php

require_once('session.php');

class client
{
	private $session = null;

	public function connect($ip, $port)
	{
		$this->session = new session();
        $obj = $this;
		$this->session->bind_connect_succ(function($session) use ($obj) { $obj->on_connect_succ($session); });
		$this->session->bind_connect_faild(function($session) use ($obj) { $obj->on_connect_faild($session); });
		$this->session->bind_connect_broken(function($session) use ($obj) { $obj->on_connect_broken($session); });
		$this->session->bind_read(function($session, $buffer) use ($obj) { $obj->on_read($session, $buffer); });
		$this->session->bind_write(function($session) use ($obj) { $obj->on_write($session); });
		$this->session->bind_timer(5, function() use ($obj) { $obj->on_timer(); });

		$this->session->connect($ip, $port);
	}

	public function on_connect_succ($session)
	{
		echo "connect success!\n";
        $this->session->send("GET / HTTP 1.1 \r\n\r\n");
	}

	public function on_connect_faild($session)
	{
		echo "connect faild!\n";
	}

	public function on_connect_broken($session)
	{
		echo "connect broken!\n";
	}

	public function on_read($session, $buffer)
	{
		echo $buffer;
	}

	public function on_write($session)
	{
		echo "write completed!\n";
	}

	public function on_timer()
	{
		echo "timer tick!\n";
	}

	public function run()
	{
        while(true)
        {
		    if($this->session->core_run() === false)
                break;
        }
	}
}

$client = new client();
$client->connect('127.0.0.1', 81);
$client->run();

?>
