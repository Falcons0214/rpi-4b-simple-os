/*
 * The address should align by 11.
 * Because 0 ~ 10 bits is reserved for VBAR_EL1.
 */
extern unsigned long vector_table_init(void);