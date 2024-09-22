#define PAGE_SHIFT	 	12
#define TABLE_SHIFT     8
#define SECTION_SHIFT	(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   	(1 << PAGE_SHIFT)	
#define SECTION_SIZE	(1 << SECTION_SHIFT)	

#define LOW_MEMORY      0x40000000
#define HIGH_MEMORY     (LOW_MEMORY + SECTION_SIZE)

#define PAGING_MEMORY   SECTION_SIZE
#define PAGING_PAGES    (PAGING_MEMORY / PAGE_SIZE)


void mm_init(void);
unsigned long get_free_page(void);
void free_page(unsigned long page);