# AMBA-3-AHB-Lite-Protocol
##### This Repo contains SystemC for testBench for AMBA® 3 AHB-Lite Protocol
AHB-Lite implements the features required for high-performance, high clock
frequency systems including:
• burst transfers
• single-clock edge operation
• non-tristate implementation
• wide data bus configurations, 64, 128, 256, 512, and 1024 bits.
The most common AHB-Lite slaves are internal memory devices, external memory
interfaces, and high bandwidth peripherals. Although low-bandwidth peripherals can be
included as AHB-Lite slaves, for system performance reasons they typically reside on
the AMBA Advanced Peripheral Bus (APB). Bridging between this higher level of bus
and APB is done using a AHB-Lite slave, known as an APB bridge.
Figure 1-1 shows a single master AHB-Lite system design with one AHB-Lite master
and three AHB-Lite slaves. The bus interconnect logic consists of one address decoder
and a slave-to-master multiplexor. The decoder monitors the address from the master so
that the appropriate slave is selected and the multiplexor routes the corresponding slave
output data back to the master
