obj-$(CONFIG_SUNXI_FRONT_END) += sunxi-front-end.o

sunxi-front-end-y = sunxi_front_end.o \
				sunxi_front_end_color_space_converter.o \
				sunxi_front_end_dma_ctrl.o				
				
				
				