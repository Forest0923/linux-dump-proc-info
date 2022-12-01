#include <linux/pgtable.h>

#define PAGE_OFFSET_MASK (~(~0UL << PAGE_SHIFT))

unsigned long page_table_walk(unsigned long addr, struct mm_struct *mm_str)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn, phys_page_addr, page_offset, phys_addr;

	pgd = pgd_offset(mm_str, addr);
	if (pgd_none(*pgd))
		return 0;

	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d))
		return 0;

	pud = pud_offset(p4d, addr);
	if (pud_none(*pud))
		return 0;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd))
		return 0;

	pte = pte_offset_map(pmd, addr);
	if (!pte || !pte_present(*pte) || pte_none(*pte) || is_zero_pfn(pte_pfn(*pte)))
	{
		return 0;
	}

	pfn = pte_pfn(*pte);
	phys_page_addr = PFN_PHYS(pfn);
	page_offset = addr & PAGE_OFFSET_MASK;
	phys_addr = phys_page_addr | page_offset;

	pte_unmap(pte);

	return phys_addr;
}

