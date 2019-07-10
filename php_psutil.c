#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_php_psutil.h"
#include "pslib.h"

/* True global resources - no need for thread safety here */
static int le_php_psutil;

PHP_FUNCTION(disk_partitions)
{
    char *arg = NULL;
    zend_bool isall;
    int i = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &isall) == FAILURE) {
        return;
    }
    DiskPartitionInfo *dp = disk_partitions((bool)!isall);  // To be compatible with psutil in Python version.
    if (dp == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    for(i = 0; i < dp->nitems; i++) {
        zval item;
        array_init(&item);
        add_assoc_string(&item, "device",       dp->partitions[i].device);
        add_assoc_string(&item, "mountpoint",   dp->partitions[i].mountpoint);
        add_assoc_string(&item, "fstype",       dp->partitions[i].fstype);
        add_assoc_string(&item, "opts",         dp->partitions[i].opts);
        add_index_zval(return_value, i, &item);
    }
    free_disk_partition_info(dp);
}

PHP_FUNCTION(disk_usage)
{
    DiskUsage du;
    zend_string *path;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &path) == FAILURE) {
        return;
    }
    if (!disk_usage(ZSTR_VAL(path), &du))
        RETURN_FALSE;
    array_init(return_value);
    add_assoc_long(return_value, "total",       du.total);
    add_assoc_long(return_value, "used",        du.used);
    add_assoc_long(return_value, "free",        du.free);
    add_assoc_double(return_value, "percent",   du.percent);
}

PHP_FUNCTION(disk_io_counters)
{
    int i = 0;
    DiskIOCounterInfo *counter = disk_io_counters();
    if (counter == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    for(i = 0; i < counter->nitems; i++) {
        zval item;
        array_init(&item);
        add_assoc_string(&item, "name",         counter->iocounters[i].name);
        add_assoc_long(&item, "readbytes",      counter->iocounters[i].readbytes);
        add_assoc_long(&item, "writebytes",     counter->iocounters[i].writebytes);
        add_assoc_long(&item, "reads",          counter->iocounters[i].reads);
        add_assoc_long(&item, "writes",         counter->iocounters[i].writes);
        add_assoc_long(&item, "readtime",       counter->iocounters[i].readtime);
        add_assoc_long(&item, "writetime",      counter->iocounters[i].writetime);
        add_index_zval(return_value, i, &item);
    }
    free_disk_iocounter_info(counter);
}

PHP_FUNCTION(net_io_counters)
{
    int i = 0;
    NetIOCounterInfo *counter = net_io_counters();
    if (counter == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    for(i = 0; i < counter->nitems; i++) {
        zval item;
        array_init(&item);
        add_assoc_string(&item, "name",         counter->iocounters[i].name);
        add_assoc_long(&item, "bytes_sent",     counter->iocounters[i].bytes_sent);
        add_assoc_long(&item, "bytes_recv",     counter->iocounters[i].bytes_recv);
        add_assoc_long(&item, "packets_sent",   counter->iocounters[i].packets_sent);
        add_assoc_long(&item, "packets_recv",   counter->iocounters[i].packets_recv);
        add_assoc_long(&item, "errin",          counter->iocounters[i].errin);
        add_assoc_long(&item, "errout",         counter->iocounters[i].errout);
        add_assoc_long(&item, "dropin",         counter->iocounters[i].dropin);
        add_assoc_long(&item, "dropout",        counter->iocounters[i].dropout);
        add_index_zval(return_value, i, &item);
    }
    free_net_iocounter_info(counter);
}

PHP_FUNCTION(get_users)
{
    int i = 0;
    UsersInfo *users = get_users();
    if (users == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    for(i = 0; i < users->nitems; i++) {
        zval item;
        array_init(&item);
        add_assoc_string(&item, "user",         users->users[i].username);
        add_assoc_string(&item, "terminal",     users->users[i].tty);
        add_assoc_string(&item, "host",         users->users[i].hostname);
        add_assoc_double(&item, "started",      users->users[i].tstamp);
        add_index_zval(return_value, i, &item);
    }
    free_users_info(users);
}

PHP_FUNCTION(boot_time)
{
    int i = 0;
    RETURN_LONG(get_boot_time());
}

PHP_FUNCTION(virtual_memory)
{
    VmemInfo vmem;
    if (!virtual_memory(&vmem))
        RETURN_FALSE;
    array_init(return_value);
    add_assoc_long(return_value, "total",       vmem.total);
    add_assoc_long(return_value, "available",   vmem.available);
    add_assoc_double(return_value, "percent",   vmem.percent);
    add_assoc_long(return_value, "used",        vmem.used);
    add_assoc_long(return_value, "free",        vmem.free);
    add_assoc_long(return_value, "active",      vmem.active);
    add_assoc_long(return_value, "inactive",    vmem.inactive);
    add_assoc_long(return_value, "buffers",     vmem.buffers);
    add_assoc_long(return_value, "cached",      vmem.cached);
    add_assoc_long(return_value, "wired",       vmem.wired);
}

PHP_FUNCTION(swap_memory)
{
    SwapMemInfo smem;
    if (!swap_memory(&smem))
        RETURN_FALSE;
    array_init(return_value);
    add_assoc_long(return_value, "total",       smem.total);
    add_assoc_long(return_value, "used",        smem.used);
    add_assoc_long(return_value, "free",        smem.free);
    add_assoc_double(return_value, "percent",   smem.percent);
    add_assoc_long(return_value, "sin",         smem.sin);
    add_assoc_long(return_value, "sout",        smem.sout);
}

PHP_FUNCTION(cpu_times)
{
    char *arg = NULL;
    zend_bool percpu;
    int i = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &percpu) == FAILURE) {
        return;
    }
    CpuTimes *ct = cpu_times((bool)percpu);
    if (ct == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    add_assoc_double(return_value, "user",          ct->user);
    add_assoc_double(return_value, "system",        ct->system);
    add_assoc_double(return_value, "idle",          ct->idle);
    add_assoc_double(return_value, "nice",          ct->nice);
    add_assoc_double(return_value, "iowait",        ct->iowait);
    add_assoc_double(return_value, "irq",           ct->irq);
    add_assoc_double(return_value, "softirq",       ct->softirq);
    add_assoc_double(return_value, "steal",         ct->steal);
    add_assoc_double(return_value, "guest",         ct->guest);
    add_assoc_double(return_value, "guest_nice",    ct->guest_nice);
    free(ct);
}

PHP_FUNCTION(cpu_count)
{
    zend_bool islogical;
    int i = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &islogical) == FAILURE) {
        return;
    }
    RETURN_LONG(cpu_count(islogical));
}

PHP_FUNCTION(pid_exists)
{
    long pid;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &pid) == FAILURE) {
        return;
    }
    if (pid_exists((pid_t)pid)) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(get_process)
{
    long pid;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &pid) == FAILURE) {
        return;
    }
    if (!pid_exists((pid_t)pid))
        RETURN_FALSE;
    Process *p = get_process((pid_t)pid);
    if (p == NULL)
        RETURN_FALSE; 
    array_init(return_value);
    add_assoc_long(return_value, "pid",             p->pid);
    add_assoc_long(return_value, "ppid",            p->ppid);
    add_assoc_string(return_value, "name",          p->name);
    add_assoc_string(return_value, "exe",           p->exe);
    add_assoc_string(return_value, "cmdline",       p->cmdline);
    add_assoc_double(return_value, "create_time",   p->create_time);
    add_assoc_long(return_value, "uid",             p->uid);
    add_assoc_long(return_value, "euid",            p->euid);
    add_assoc_long(return_value, "suid",            p->suid);
    add_assoc_long(return_value, "gid",             p->gid);
    add_assoc_long(return_value, "egid",            p->egid);
    add_assoc_long(return_value, "sgid",            p->sgid);
    add_assoc_string(return_value, "username",      p->username);
    add_assoc_string(return_value, "terminal",      p->terminal);
    free_process(p);
}

PHP_MINIT_FUNCTION(php_psutil)
{
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(php_psutil)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(php_psutil)
{
#if defined(COMPILE_DL_PHP_PSUTIL) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(php_psutil)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(php_psutil)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "php_psutil support", "enabled");
    php_info_print_table_end();
}

const zend_function_entry php_psutil_functions[] = {
    PHP_FE(disk_partitions,     NULL)
    PHP_FE(disk_usage,          NULL)
    PHP_FE(disk_io_counters,    NULL)
    PHP_FE(net_io_counters,     NULL)
    PHP_FE(get_users,           NULL)
    PHP_FE(boot_time,           NULL)
    PHP_FE(virtual_memory,      NULL)
    PHP_FE(swap_memory,         NULL)
    PHP_FE(cpu_times,           NULL)
    PHP_FE(cpu_count,           NULL)
    PHP_FE(pid_exists,          NULL)
    PHP_FE(get_process,         NULL)
    PHP_FE_END
};

zend_module_entry php_psutil_module_entry = {
    STANDARD_MODULE_HEADER,
    "php_psutil",
    php_psutil_functions,
    PHP_MINIT(php_psutil),
    PHP_MSHUTDOWN(php_psutil),
    PHP_RINIT(php_psutil),
    PHP_RSHUTDOWN(php_psutil),
    PHP_MINFO(php_psutil),
    PHP_PHP_PSUTIL_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PHP_PSUTIL
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(php_psutil)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
