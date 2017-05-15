/*
 * libusb example program to list devices on the bus
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#include <libusb.h>



static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;
    libusb_device_handle *handle;
    unsigned char data[256]={0};
    int ret = 0;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d, class %d)\n",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev), desc.iProduct);
        
        if(desc.iProduct) {
            libusb_open(dev, &handle);
            ret = libusb_get_string_descriptor_ascii(handle, desc.iProduct, data, 256);
            
            if (ret > 0) {
                data[ret] = 0;
                printf("product: %s\n", data);
                if ( !strncmp((const char*)data, "P4000DN", ret) ) {
                    
                }
            }
            
            libusb_close(handle);
        }
	}
}

int print_prn()
{
    int r;
    FILE *f;
    unsigned char buffer[8192] = {0};
    size_t read_byte;
    int write_byte;
    libusb_device_handle *handle;
    handle = libusb_open_device_with_vid_pid(NULL, 0x043d, 0x022c);
    if (!handle) {
        printf("open device error\n");
        return -1;
    }
	if (libusb_kernel_driver_active(handle, 0) == 1) {
	  if (libusb_detach_kernel_driver(handle, 0) == 0) {
	  }
	}
    r = libusb_claim_interface(handle, 0);
    if (r < 0) {
        printf("Can not claim insterface (%s)\n", libusb_error_name(r));
    }
    
    f = fopen("cp2500/hp_600dpi.prn", "rb");
    do {
        read_byte = fread(buffer, 1, 8192, f);
        if (read_byte > 0) {
            printf("Read %zu\n", read_byte);
            r = libusb_bulk_transfer(handle, 1, buffer, read_byte, &write_byte, 3000);
            if (r < 0) {
                printf("bulk transfer error (%s)", libusb_error_name(r));
            }
            
        } else {
            printf("Read error (%zu)!\n", read_byte);
            break;
        }
    } while (1);
    libusb_close(handle);
    fclose(f);
    return 0;
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;
    print_prn();
    /*
	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);*/

	libusb_exit(NULL);
	return 0;
}

