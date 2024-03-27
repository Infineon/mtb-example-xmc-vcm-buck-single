# XMC&trade; MCU: VCM buck converter

This example implements a buck converter in voltage control mode for being used with the XMC&trade; Digital Power Explorer Board. The XMC&trade; Digital Power Explorer Board can be used with XMC1300 Digital Power Control Card or XMC4200 Digital Power Control Card. 

This example contains configurations for running on both these drive cards. The XMC1300 Digital Power Control Card is with a 32-bit Arm&reg; Cortex&reg;-M0 based microcontroller (XMC1302) and XMC4200 Digital Power Control Card is with a 32-bit Arm&reg; Cortex&reg;-M4 based microcontroller with floating-point unit (XMC4200). 



## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.1
- Board support package (BSP) minimum required version: (KIT_XMC13_DPCC_V1) v2.3.0
- Board support package (BSP) minimum required version: (KIT_XMC42_DPCC_V1) v2.3.0
- [SEGGER J-Link software](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- Programming language: C
- Associated parts: All [XMC&trade; MCU](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/) parts

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v10.3.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`

## Supported kits (make variable 'TARGET')

- [XMC4200 Digital Power Explorer Control Card](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/) (`KIT_XMC42_DPCC_V1`)
- [XMC1300 Digital Power Explorer Control Card](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/) (`KIT_XMC13_DPCC_V1`)

## Hardware setup

You can use this example in the following configurations: 
 - XMC1300 Digital Power Control Card + XMC&trade; Digital Power Explorer Board: While using this configuration, select KIT_XMC13_DPCC_V1 as the BSP.
   
 - XMC4200 Digital Power Control Card + XMC&trade; Digital Power Explorer Board: While using this configuration, select KIT_XMC42_DPCC_V1 as the BSP.

## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.

This example requires no additional software or tools.

## Using the code example

### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool.

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*).

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target).

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**.

   > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you.

   b.	Select this code example from the list by enabling its check box.

   > **Note:** You can narrow the list of displayed examples by typing in the filter box.

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**.

   d. Click **Create** to complete the application creation process.

</details>

<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[mtb-example-xmc-vcm-buck-single](https://github.com/Infineon/mtb-example-xmc-vcm-buck-single)" application with the desired name "VCM_buck" configured for the *KIT_XMC42_DPCC_V1* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id KIT_XMC42_DPCC_V1 --app-id mtb-example-xmc-vcm-buck-single --user-app-name VCM_buck --target-dir "C:/mtb_projects"
   ```


The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>



### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

## Operation

You can use this code example with the KIT_XMC13_DPCC_V1 or KIT_XMC42_DPCC_V1 board paired with XMC&trade; Digital Power Explorer Board. If you use this code example with KIT_XMC42_DPCC_V1, connect the board to the XMC&trade; Digital Power Explorer Board and set the SV5 jumper to the XMC4000 option. If you use this code example with KIT_XMC13_DPCC_V1, connect the board to the XMC&trade; Digital Power Explorer Board and set the jumper SV5 to the XMC1000 option.

To provide the input power, XMC&trade; Digital Power Explorer Board comes with a 12 V adaptor. Connect the adaptor output to the 12 V DC Supply Jack (J2) of the XMC&trade; Digital Power Explorer Board. The regulated output voltage is available in the testpoint marked 'VOUT FILT' on the XMC&trade; Digital Power Explorer Board. To monitor the regulated output voltage, connect an oscilloscope or multimeter to this point.

1. Connect the board to your PC using the provided USB cable through the JLink USB connector.

2. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (JLink)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

3. After programming, the application starts automatically.

4. Check that the XMC&trade; Digital Power Explorer Board produces 3.3 V output.

5. Try varying the load on the output using the switches SW1, SW2, and SW3 and verify that the output voltage is maintained. 

## Debugging

You can debug the example to step through the code.

<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (JLink)** configuration in the **Quick Panel**. For details, see the Program and debug section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>

## Design and implementation


### XMC1302 MCU configuration

For the XMC1302 MCU, the control loop is implemented by a 3-poles 3-zeros filter using fixed-point values. The filter coefficients have been selected to have the following controller characteristics.

**Table 1. Filter coefficients for XMC1302 MCU**

Description |  Value
:-----------|  :---
Vout        |  3.3 V
Switching freq | 100 kHz
Crossover freq |  5 kHz
Phase margin   |  50 degrees
PWM master clock  |  64 MHz
ADC resolution |  12 bits
Max duty |  90%
 
<br>

Filter-related APIs are implemented in the *xmc_3p3z_filter_fixed.h* file and the PWM initialization and control ISR are implemented in the *xmc13_vcm_buck_single.c* file.


### XMC4200 MCU configuration

For the XMC4200 MCU, the control loop is implemented by a 3-poles 3-zeros filter using floating-point values. The filter coefficients have been selected to have the following controller characteristics.

**Table 2. Filter coefficients for XMC4200 MCU**

Description |  Value
:-----------|  :---
Vout        |  3.3 V
Switching freq | 200 kHz
Crossover freq |  10 kHz
Phase margin   |  50 degrees
PWM master clock  |  20,480 GHz = 80 MHz *256 (8 bits for HRPWM)
ADC resolution |  12 bits
Max duty |  90%

<br>

Filter-related APIs are implemented in the *xmc_3p3z_filter_float.h* file and the PWM initialization and control ISR are implemented in the *xmc42_vcm_buck_single.c* file.

Select the required BSP based on the drive card used.

### Resources and settings

**Table 3. Application resources on KIT_XMC13_DPCC_V1**

Resource  |  Alias/object     |    Purpose
:-------- | :-------------    | :------------
PWM       |  P0_0             | PWM Direct Output
PWM Compl |  P0_5             | PWM Complementary Output
ADC Vout  |  P2_3             | ADC Analog Input (Output Voltage)

<br>

**Table 4. Application resources on KIT_XMC42_DPCC_V1**

Resource  |  Alias/object     |    Purpose
:-------- | :-------------    | :------------
PWM       | P0_5              | PWM direct output
PWM Compl | P0_2              | PWM complementary output
ADC Vout  | P14_6             | ADC analog input (output voltage)

<br>

## Related resources


Resources  | Links
-----------|----------------------------------
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Kit guides | [XMC1300 Digital Power Control Card user manual](https://www.infineon.com/dgdl/Infineon-XMC1300_DigitaPowerControlCard_UserManual-UM-v01_00-EN.pdf?fileId=5546d4625185e0e201518be86d193e29) <br> [XMC4200 Digital Power Control Card user manual](https://www.infineon.com/dgdl/Infineon-XMC4200_Microcontroller_Digital_Power_Control_Card_User_Manual-UM-v01_01-EN.pdf?fileId=5546d4625185e0e201518bf18e663e3b)
Device documentation | [XMC1000 family datasheets](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc1000-industrial-microcontroller-arm-cortex-m0/#document-group-myInfineon-49) <br> [XMC1000 family technical reference manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc1000-industrial-microcontroller-arm-cortex-m0/#document-group-myInfineon-44) <br> [XMC4000 family datasheets](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-49) <br> [XMC4000 family technical reference manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/#document-group-myInfineon-44)
Development kits | [XMC&trade; eval boards](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/#boards)
Libraries on GitHub  | [mtb-xmclib-cat3](https://github.com/Infineon/mtb-xmclib-cat3) – XMC&trade; peripheral driver library (XMCLib) and docs
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSoC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.

For XMC&trade; MCU devices, see [32-bit XMC&trade; industrial microcontroller based on Arm&reg; Cortex&reg;-M](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/).

## Document history

Document title: *CE239198* – *XMC&trade; MCU: VCM buck converter*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example     

------

All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.


© 2023 Infineon Technologies AG

All Rights Reserved.

### Legal disclaimer

The information given in this document shall in no event be regarded as a guarantee of conditions or characteristics. With respect to any examples or hints given herein, any typical values stated herein and/or any information regarding the application of the device, Infineon Technologies hereby disclaims any and all warranties and liabilities of any kind, including without limitation, warranties of non-infringement of intellectual property rights of any third party.

### Information

For further information on technology, delivery terms and conditions and prices, please contact the nearest Infineon Technologies Office (www.infineon.com).

### Warnings

Due to technical requirements, components may contain dangerous substances. For information on the types in question, please contact the nearest Infineon Technologies Office.

Infineon Technologies components may be used in life-support devices or systems only with the express written approval of Infineon Technologies, if a failure of such components can reasonably be expected to cause the failure of that life-support device or system or to affect the safety or effectiveness of that device or system. Life support devices or systems are intended to be implanted in the human body or to support and/or maintain and sustain and/or protect human life. If they fail, it is reasonable to assume that the health of the user or other persons may be endangered.

-------------------------------------------------------------------------------