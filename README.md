Summary
=======

php_psutil is PHP version of psutil for system(CPU, memory, disks, network) and process information. It is used to monitor the system status. php_psutil depends on [cpslib](https://github.com/hamon-in/cpslib) and the API is mostly compatible with psutil in Python version.



APIs
=======

Disk
---

  * `disk_partitions(all)`
  * `disk_io_counters()`
  * `disk_usage()`

Network
---

  * `net_io_counters()`

Users
---

  * `get_users()`

Boot time
---

  * `boot_time()`

Memory
---

  * `virtual_memory()`
  * `swap_memory()`

CPU
---

  * `cpu_times(percpu)`
  * `cpu_count(logical)`

Process
---

  * `pid_exists(pid)`
  * `get_process(pid)`

Example Usage
=======

```PHP
<?php
$dp = disk_partitions(false);
var_dump($dp);

$dp = disk_partitions(true);
var_dump($dp);

$du = disk_usage("/");
var_dump($du);

$counter = disk_io_counters();
var_dump($counter);

$counter = net_io_counters();
var_dump($counter);

$users = get_users();
var_dump($users);

$boottime = boot_time();
var_dump($boottime);

$vmem = virtual_memory();
var_dump($vmem);

$smem = swap_memory();
var_dump($smem);

$ct = cpu_times(false);
var_dump($ct);

$ct = cpu_times(true);
var_dump($ct);

$cc = cpu_count(false);
var_dump($cc);

$cc = cpu_count(True);
var_dump($cc);

$b = pid_exists(1);
var_dump($b);

$p = get_process(3636);
var_dump($p);
?>
```
