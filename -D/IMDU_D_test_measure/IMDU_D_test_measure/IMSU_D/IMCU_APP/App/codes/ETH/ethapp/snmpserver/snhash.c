/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   snhash.c                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */
#include "kernel.h"

#include "snmp.h"
#include "mib.h"
//#include "snhash.h"

//struct	snhnode	*snhtab[S_HTABSIZ];

//extern struct tab_info	tabtab[];

/*------------------------------------------------------------------------
 * getmib - find mib record for the given object id
 *------------------------------------------------------------------------
 */
struct mib_info *
getmib(struct oid *oip)
{
	struct mib_info *hp;
	int		loc;
     loc = 0;

	for (hp = mib; loc<mib_entries ; hp++,loc++) 
	{
		
		if (oidequ(oip, &hp->mi_objid))
		{
			return hp;
		}
	}

	return 0;
}

/*------------------------------------------------------------------------
 * hashoid - hash the object id
 *------------------------------------------------------------------------
 */

//#endif	/* SNMP */
