/****************************************************************************
 * main.c
 *
 *   Copyright (C) 2021 Daniel P. Carvalho. All rights reserved.
 *   Authors: Daniel P. Carvalho <daniel.carvalho@ufu.br>
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdlib.h>

 /****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* AHB1 Base Addresses ******************************************************/

#define STM32_RCC_BASE       0x40023800     /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* GPIOC Base Addresses ******************************************************/

#define STM32_GPIOC_BASE     0x40020800     /* 0x48000800-0x48000bff: GPIO Port C */
#define STM32_GPIOA_BASE     0x40020000     /* 0x40020000-0x5fffffff: GPIO Port A */


/* Register Offsets *********************************************************/

#define STM32_RCC_AHB1ENR_OFFSET  0x0030   /* AHB1 Peripheral Clock enable register */

#define STM32_GPIO_MODER_OFFSET   0x0000  /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET  0x0004  /* GPIO port output type register */
#define STM32_GPIO_PUPDR_OFFSET   0x000c  /* GPIO port pull-up/pull-down register */
#define STM32_GPIO_IDR_OFFSET     0x0010  /* GPIO port bit input data register */
#define STM32_GPIO_ODR_OFFSET     0x0014  /* GPIO port output data register */
#define STM32_GPIO_BSRR_OFFSET    0x0018  /* GPIO port bit set/reset register */


/* Register Addresses *******************************************************/

#define STM32_RCC_AHB1ENR        (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOC_MODER        (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER       (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR        (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR          (STM32_GPIOC_BASE+STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR         (STM32_GPIOC_BASE+STM32_GPIO_BSRR_OFFSET)
// #define STM32_GPIOC_IDR          (STM32_GPIOC_BASE+STM32_GPIO_IDR_OFFSET)

#define STM32_GPIOA_MODER        (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_OTYPER       (STM32_GPIOA_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOA_PUPDR        (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOA_ODR          (STM32_GPIOA_BASE+STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOA_IDR          (STM32_GPIOA_BASE+STM32_GPIO_IDR_OFFSET)


/* AHB1 Peripheral Clock enable register */

#define RCC_AHB1ENR_GPIOCEN      (1 << 2)  /* Bit 2:  IO port C clock enable */
#define RCC_AHB1ENR_GPIOAEN      (1 << 0)  /* Bit 0:  IO port A clock enable */


/* GPIO port mode register */

#define GPIO_MODER_INPUT           (0) /* Input */
#define GPIO_MODER_OUTPUT          (1) /* General purpose output mode */
#define GPIO_MODER_ALT             (2) /* Alternate mode */
#define GPIO_MODER_ANALOG          (3) /* Analog mode */

#define GPIO_MODER_SHIFT(n)        (n << 1)
#define GPIO_MODER_MASK(n)         (3 << GPIO_MODER_SHIFT(n))

/* GPIO port output type register */

#define GPIO_OTYPER_PP             (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD             (1) /* 1=Output open-drain */

#define GPIO_OT_SHIFT(n)           (n)
#define GPIO_OT_MASK(n)            (1 << GPIO_OT_SHIFT(n))

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE            (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP          (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN        (2) /* Pull-down */

#define GPIO_PUPDR_SHIFT(n)        (n << 1)
#define GPIO_PUPDR_MASK(n)         (3 << GPIO_PUPDR_SHIFT(n))

/* GPIO port bit set/reset register */

#define GPIO_BSRR_SET(n)           (1 << (n))
#define GPIO_BSRR_RESET(n)         (1 << ((n) + 16))


/* GPIO input data register - read data */
#define GPIO_IDR_MASK(n)           (1 << (n))


/* Configuration ************************************************************/

#define LED_DELAY  100000
#define LED_PC13   13
#define BTN_PA0    0
#define HIGH       1
#define LOW        0


/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

 /****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char *argv[])
{
  uint32_t i;
  uint32_t reg;

  /* --------------------- PINO COM O LED (PC13) -------------------- */
  /* Ponteiros para registradores */

  uint32_t *pRCC_AHB1ENR  = (uint32_t *)STM32_RCC_AHB1ENR;
  uint32_t *pGPIOC_MODER  = (uint32_t *)STM32_GPIOC_MODER;
  uint32_t *pGPIOC_OTYPER = (uint32_t *)STM32_GPIOC_OTYPER;
  uint32_t *pGPIOC_PUPDR  = (uint32_t *)STM32_GPIOC_PUPDR;
  uint32_t *pGPIOC_BSRR   = (uint32_t *)STM32_GPIOC_BSRR;
  // uint32_t *pGPIOC_IDR    = (uint32_t *)STM32_GPIOC_IDR;

  /* Habilita clock GPIOC */

  reg  = *pRCC_AHB1ENR;
  reg |= RCC_AHB1ENR_GPIOCEN;
  *pRCC_AHB1ENR = reg;

  /* Configura PC13 como saida pull-up off e pull-down off */

  reg = *pGPIOC_MODER;
  reg &= ~GPIO_MODER_MASK(LED_PC13);
  reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(LED_PC13));
  *pGPIOC_MODER = reg;  

  reg = *pGPIOC_OTYPER;
  reg &= ~GPIO_OT_MASK(LED_PC13);
  reg |= (GPIO_OTYPER_PP << GPIO_OT_SHIFT(LED_PC13));
  *pGPIOC_OTYPER = reg;

  reg = *pGPIOC_PUPDR;
  reg &= ~GPIO_PUPDR_MASK(LED_PC13);
  reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(LED_PC13));
  *pGPIOC_PUPDR = reg;


  /* --------------------- PINO COM O BOTÃO (PA0) -------------------- */
  /* Ponteiro para os registradores da porta A */
  uint32_t *pGPIOA_MODER  = (uint32_t *)STM32_GPIOA_MODER;
  uint32_t *pGPIOA_PUPDR  = (uint32_t *)STM32_GPIOA_PUPDR;
  uint32_t *pGPIOA_IDR    = (uint32_t *)STM32_GPIOA_IDR;

  /* Habilita clock da porta A */
  reg = *pRCC_AHB1ENR;
  reg |= RCC_AHB1ENR_GPIOAEN;
  *pRCC_AHB1ENR = reg;

  /* Configura o PA0 como entrada */
  reg = *pGPIOA_MODER;
  reg &= ~GPIO_MODER_MASK(BTN_PA0);
  reg |= (GPIO_MODER_INPUT << GPIO_MODER_SHIFT(BTN_PA0));
  *pGPIOA_MODER = reg;

  /* Configura o PA0 sem pull-up e sem pull-down internos */
  reg = *pGPIOA_PUPDR;
  reg &= ~GPIO_PUPDR_MASK(BTN_PA0);
  reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(BTN_PA0));
  *pGPIOA_PUPDR = reg;

  uint8_t led_ligado = 1;
  // uint8_t valor;

  while(1) {
      /* Liga LED */
      // *pGPIOC_BSRR = GPIO_BSRR_RESET(13);
      // for (i = 0; i < LED_DELAY; i++);

      /* Desliga LED */
      // *pGPIOC_BSRR = GPIO_BSRR_SET(13);
      // for (i = 0; i < LED_DELAY; i++);

      /* Botão apertado */
      // if( ((*pGPIOA_IDR) & GPIO_IDR_MASK(BTN_PA0)) == LOW ) {
      //   *pGPIOC_BSRR = GPIO_BSRR_RESET(LED_PC13);
      // }
      // else if ( ((*pGPIOA_IDR) & GPIO_IDR_MASK(BTN_PA0)) == HIGH ) {
      //   *pGPIOC_BSRR = GPIO_BSRR_SET(LED_PC13);
      // }

      /* Botão apertado */
      if( (*pGPIOA_IDR) & GPIO_IDR_MASK(BTN_PA0) == LOW ) {

        // valor = ((*pGPIOC_IDR) & GPIO_IDR_MASK(LED_PC13));
        // if(valor == 1) {
        //   *pGPIOC_BSRR = GPIO_BSRR_RESET(LED_PC13);
        // }
        // else if(valor == 0) {
        //   *pGPIOC_BSRR = GPIO_BSRR_SET(LED_PC13);
        // }

        if(led_ligado == 0) {
          *pGPIOC_BSRR = GPIO_BSRR_SET(LED_PC13);
          led_ligado = 1;
        }
        else if(led_ligado == 1) {
          *pGPIOC_BSRR = GPIO_BSRR_RESET(LED_PC13);
          led_ligado = 0;
        }
      }

      for(i = 0; i < LED_DELAY; i++);
  }

  /* Nunca deveria chegar aqui */

  return EXIT_FAILURE;
}