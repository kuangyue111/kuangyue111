/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "pit_hw_access.h"

/******************************************************************************
 * Code
 *****************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_Reset
 * Description   : Set control, load and status registers to default value.
 * This function sets control, load and status registers to default value.
 *
 *END**************************************************************************/
void PIT_Reset(PIT_Type * const base, const uint8_t channelNum)
{
    uint32_t i;
    uint32_t mask;

    for (i = 0U; i < channelNum; i++)
    {
        base->TIMER[i].TCTRL = PIT_TCTRL_TEN(0U) | PIT_TCTRL_TIE(0U) | PIT_TCTRL_CHN(0U);
        base->TIMER[i].LDVAL = PIT_LDVAL_TSV(0U);
        base->TIMER[i].TFLG = PIT_TFLG_TIF_MASK;
    }

    mask = PIT_MCR_MDIS(PIT_MCR_MDIS_DEFAULT) | PIT_MCR_FRZ(0U);

#if FEATURE_PIT_HAS_RTI_CHANNEL
    base->RTI_TCTRL = PIT_RTI_TCTRL_TEN(0U) | PIT_RTI_TCTRL_TIE(0U);
    base->RTI_LDVAL = PIT_RTI_LDVAL_TSV(0U);
    base->RTI_TFLG = PIT_RTI_TFLG_TIF_MASK;
    base->MCR = PIT_MCR_MDIS_RTI(PIT_MCR_MDIS_RTI_DEFAULT) | mask;
#else
    base->MCR = mask;
#endif

}
/*******************************************************************************
 * EOF
 ******************************************************************************/
