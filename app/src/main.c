#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

K_MSGQ_DEFINE(q, sizeof(int), 8, 4);

void producer(void *a, void *b, void *c) {
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);
    int value = 0;
    while (1) {
        value++;
        k_msgq_put(&q, &value, K_FOREVER);
        printk("[producer] -> %d\n", value);
        k_sleep(K_MSEC(500));
    }
}

void consumer(void *a, void *b, void *c) {
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);
    int rx;
    while (1) {
        if (k_msgq_get(&q, &rx, K_MSEC(1000)) == 0) {
            printk("              [consumer] got %d\n", rx);
        } else {
            printk("              [consumer] timeout\n");
        }
    }
}

K_THREAD_STACK_DEFINE(prod_stack, 1024);
K_THREAD_STACK_DEFINE(cons_stack, 1024);
static struct k_thread prod_tid, cons_tid;

int main(void) {
    printk("Zephyr + QEMU RTOS Playground\n");
    k_thread_create(&prod_tid, prod_stack, K_THREAD_STACK_SIZEOF(prod_stack),
                    producer, NULL, NULL, NULL, 3, 0, K_NO_WAIT);
    k_thread_create(&cons_tid, cons_stack, K_THREAD_STACK_SIZEOF(cons_stack),
                    consumer, NULL, NULL, NULL, 4, 0, K_NO_WAIT);
    return 0;
}
