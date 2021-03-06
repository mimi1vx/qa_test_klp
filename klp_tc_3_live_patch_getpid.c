/*
 * live_patch_getpid - patch getpid with the same code
 *
 *  Copyright (c) 2017 SUSE
 *   Author: Libor Pechacek
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/livepatch.h>

/*
 * This all should be autogenerated from the patched sources
 */

asmlinkage long klp_test_sys_getpid(void)
{
	return task_tgid_vnr(current);
}

static struct klp_func funcs[] = {
        {
                .old_name = "sys_getpid",
                .new_func = klp_test_sys_getpid,
        }, { }
};

static struct klp_object objs[] = {
        {
                /* name being NULL means vmlinux */
                .funcs = funcs,
        }, { }
};

static struct klp_patch patch = {
        .mod = THIS_MODULE,
        .objs = objs,
};


static int livepatch_init(void)
{
        int ret;

        ret = klp_register_patch(&patch);
        if (ret)
                return ret;
        ret = klp_enable_patch(&patch);
        if (ret) {
                WARN_ON(klp_unregister_patch(&patch));
                return ret;
        }
        return 0;
}

static void livepatch_exit(void)
{
        WARN_ON(klp_disable_patch(&patch));
        WARN_ON(klp_unregister_patch(&patch));
}

module_init(livepatch_init);
module_exit(livepatch_exit);
MODULE_LICENSE("GPL");
MODULE_INFO(livepatch, "Y");
