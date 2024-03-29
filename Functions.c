/*
 * Functions.c
 *
 *  Created on: 13/05/2022
 *      Author: Nelida Hernández
 */

#include "Functions.h"
#include "GPIO.h"

AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t tcdMemoryPoolPtr[TCD_QUEUE_SIZE + 1], sizeof(edma_tcd_t));

edma_handle_t g_EDMA_Handle;
uint8_t g_porta = FALSE;
boolean_t g_Transfer_Done = FALSE;
#define ARRAYS_LENGTH (200U)

//Tables that contains pre-calculated values for three signals

static uint16_t triangle_values[ARRAYS_LENGTH] = {
		0U, 41U, 82U, 123U, 165U, 206U, 247U, 288U, 329U, 370U, 412U, 453U, 494U, 535U, 576U, 617U, 658U, 700U,
		741U, 782U, 823U, 864U, 905U, 947U, 988U, 1029U, 1070U, 1111U, 1152U, 1194U, 1235U, 1276U, 1317U, 1358U,
		1399U, 1440U, 1482U, 1523U, 1564U, 1605U, 1646U, 1687U, 1729U, 1770U, 1811U, 1852U, 1893U, 1934U, 1975U,
		2017U, 2058U, 2099U, 2140U, 2181U, 2222U, 2264U, 2305U, 2346U, 2387U, 2428U, 2469U, 2511U, 2552U, 2593U,
		2634U, 2675U, 2716U, 2757U, 2799U, 2840U, 2881U, 2922U, 2963U, 3004U, 3046U, 3087U, 3128U, 3169U, 3210U,
		3251U, 3292U, 3334U, 3375U, 3416U, 3457U, 3498U, 3539U, 3581U, 3622U, 3663U, 3704U, 3745U, 3786U, 3827U,
		3869U, 3910U, 3951U, 3992U, 4033U, 4074U, 4074U, 4033U, 3992U, 3951U, 3910U, 3869U, 3827U, 3786U, 3745U,
		3704U, 3663U, 3622U, 3581U, 3539U, 3498U, 3457U, 3416U, 3375U, 3334U, 3292U, 3251U, 3210U, 3169U, 3128U,
		3087U, 3046U, 3004U, 2963U, 2922U, 2881U, 2840U, 2799U, 2757U, 2716U, 2675U, 2634U, 2593U, 2552U, 2511U,
		2469U, 2428U, 2387U, 2346U, 2305U, 2264U, 2222U, 2181U, 2140U, 2099U, 2058U, 2017U, 1975U, 1934U, 1893U,
		1852U, 1811U, 1770U, 1729U, 1687U, 1646U, 1605U, 1564U, 1523U, 1482U, 1440U, 1399U, 1358U, 1317U, 1276U,
		1235U, 1194U, 1152U, 1111U, 1070U, 1029U, 988U,  947U,  905U,  864U,  823U,  782U,  741U,  700U,  658U,
		617U,  576U,  535U,  494U,  453U,  412U,  370U,  329U,  288U,  247U,  206U,  165U,  123U,  82U,   41U,  0U
};

static uint16_t sawtooth_values[ARRAYS_LENGTH] = {
		0U, 24U, 48U, 72U, 96U, 120U, 145U, 169U, 193U, 217U, 241U, 265U, 289U, 313U, 337U, 361U, 386U, 410U, 434U,
		458U, 482U, 506U, 530U, 554U, 578U, 602U, 626U, 651U, 675U, 699U, 723U, 747U, 771U, 795U, 819U, 843U, 867U,
		891U, 916U, 940U, 964U, 988U, 1012U, 1036U, 1060U, 1084U, 1108U, 1132U, 1157U, 1181U, 1205U, 1229U, 1253U,
		1277U, 1301U, 1325U, 1349U, 1373U, 1397U, 1422U, 1446U, 1470U, 1494U, 1518U, 1542U, 1566U, 1590U, 1614U,
		1638U, 1662U, 1687U, 1711U, 1735U, 1759U, 1783U, 1807U, 1831U, 1855U, 1879U, 1903U, 1928U, 1952U, 1976U,
		2000U, 2024U, 2048U, 2072U, 2096U, 2120U, 2144U, 2168U, 2193U, 2217U, 2241U, 2265U, 2289U, 2313U, 2337U,
		2361U, 2385U, 2409U, 2434U, 2458U, 2482U, 2506U, 2530U, 2554U, 2578U, 2602U, 2626U, 2650U, 2674U, 2699U,
		2723U, 2747U, 2771U, 2795U, 2819U, 2843U, 2867U, 2891U, 2915U, 2939U, 2964U, 2988U, 3012U, 3036U, 3060U,
		3084U, 3108U, 3132U, 3156U, 3180U, 3205U, 3229U, 3253U, 3277U, 3301U, 3325U, 3349U, 3373U, 3397U, 3421U,
		3445U, 3470U, 3494U, 3518U, 3542U, 3566U, 3590U, 3614U, 3638U, 3662U, 3686U, 3710U, 3735U, 3759U, 3783U,
		3807U, 3831U, 3855U, 3879U, 3903U, 3927U, 3951U, 3976U, 4000U, 4024U, 4048U, 4072U, 3935U, 3799U, 3662U,
		3526U, 3389U, 3253U, 3116U, 2980U, 2843U, 2707U, 2570U, 2434U, 2297U, 2160U, 2024U, 1887U, 1751U, 1614U,
		1478U, 1341U, 1205U, 1068U, 932U,  795U,  659U,  522U,  386U,  249U,  112U,  0U,
};

static uint16_t sin_values[ARRAYS_LENGTH] = {
		2048U, 2113U, 2177U, 2242U, 2306U, 2370U, 2434U, 2497U, 2560U, 2622U, 2684U, 2745U, 2806U, 2865U, 2924U,
		2982U, 3039U, 3095U, 3150U, 3204U, 3257U, 3309U, 3359U, 3408U, 3456U, 3502U, 3547U, 3590U, 3632U, 3672U,
		3711U, 3747U, 3783U, 3816U, 3848U, 3878U, 3906U, 3932U, 3957U, 3979U, 4000U, 4018U,	4035U, 4050U, 4062U,
		4073U, 4082U, 4088U, 4093U, 4095U, 4095U, 4094U, 4091U, 4085U, 4078U, 4068U, 4056U, 4043U, 4027U, 4009U,
		3990U, 3968U, 3945U, 3919U, 3892U, 3863U, 3832U, 3800U, 3765U, 3729U, 3691U, 3652U, 3611U, 3569U, 3524U,
		3479U, 3432U, 3384U, 3334U, 3283U, 3231U, 3177U, 3123U, 3067U, 3011U, 2953U, 2895U, 2835U, 2775U, 2715U,
		2653U, 2591U, 2528U, 2465U, 2402U, 2338U, 2274U, 2209U, 2145U, 2080U, 2016U, 1951U, 1887U, 1822U, 1758U,
		1694U, 1631U, 1568U, 1505U, 1443U, 1381U, 1321U, 1261U, 1201U, 1143U, 1085U, 1029U, 973U,  919U,  865U,
		813U,  762U,  712U,  664U,  617U,  572U,  527U,  485U,  444U,  405U,  367U,  331U,  296U,  264U,  233U,
		204U,  177U,  151U,  128U,  106U,  87U,   69U,   53U,   40U,   28U,   18U,   11U,   5U,    2U,    0U,
		1U,    3U,    8U,    14U,   23U,   34U,   46U,	 61U,	78U,   96U,	  117U,  139U,  164U,  190U,  218U,
		248U,  280U,  313U,  349U,  385U,  424U,  464U,  506U,  549U,  594U,  640U,  688U,  737U,  787U,  839U,
		892U,  946U,  1001U, 1057U, 1114U, 1172U, 1231U, 1290U, 1351U, 1412U, 1474U, 1536U, 1599U, 1662U, 1726U,
		1790U, 1854U, 1919U, 1983U, 2048U
};

void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

void configure_transm(void)
{
	edma_transfer_config_t transferConfig;
	edma_config_t userConfig;
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;
    DMAMUX_Init(DMAMUX0);

    DMAMUX_EnablePeriodTrigger(DMAMUX0, 0);
    DMAMUX_SetSource(DMAMUX0, 0, 58);

    DMAMUX_EnableChannel(DMAMUX0, 0);
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DMA0, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    EDMA_ResetChannel(g_EDMA_Handle.base, g_EDMA_Handle.channel);

    EDMA_PrepareTransferConfig(&transferConfig,
    		   &var_array[0],
			   4,
			   4, /* source offset */
			   DAC0_BASE,
			   4,
			   0,               /* dest offset */
			   4,             /* minor loop bytes: 8*/
			   4095*4); /* major loop counts : 4 */
   EDMA_TcdSetMajorOffsetConfig(
		    		   tcdMemoryPoolPtr, //param tcd A point to the TCD structure.
					   -4095*4, //* param sourceOffset source address offset.
					   0);//destOffset destination address offset.

   EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, NULL);

   EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);
   EDMA_TcdEnableAutoStopRequest(&tcdMemoryPoolPtr[0], false);
   EDMA_InstallTCD(DMA0, 0, &tcdMemoryPoolPtr[0]);
   EDMA_EnableChannelRequest(DMA0, 0);
}
