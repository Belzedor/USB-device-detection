#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include <libusb.h>
#include <iostream>

static void print_dev(libusb_device *dev)
{
    struct libusb_device_descriptor desc;
    int err = libusb_get_device_descriptor(dev, &desc);
    if (err < 0)
    {
        fprintf(stderr, "failed to get device descriptor");
        return;
    }

    printf("VID: %04x PID: %04x\nBus: %d\nHub: %d\n",
        desc.idVendor, desc.idProduct,
        libusb_get_bus_number(dev), libusb_get_device_address(dev));

    std::cout <<"Port: ";
    if (int port = libusb_get_port_number(dev))
    {
        std::cout << port << '\n';
    }
    else
    {
        std::cout <<"no data"<< '\n';
    }

    libusb_device_handle *dev_handle;
    libusb_open(dev, &dev_handle);
    unsigned char string[200];

    std::cout << "Vendor: " ;
    if (desc.iManufacturer && libusb_get_string_descriptor_ascii(dev_handle, desc.iManufacturer, string, 200) >= 0)
    {
        std::cout << string << '\n';
    }
    else
    {
        std::cout << "no data" << '\n';
    }

    std::cout << "Product: " ;
    if (desc.iProduct && libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, string, 200) >= 0)
    {
        std::cout << string << '\n';
    }
    else
    {
        std::cout << "no data" << '\n';
    }

    std::cout << "Serial: " ;
    if (desc.iSerialNumber)
    {
        libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, string, 200);
        std::cout << string << '\n';
    }
    else
    {
        std::cout << "no data" << '\n';
    }
}

int main(void)
{
    libusb_device **current_devs;
    libusb_device **new_devs;
    libusb_device *current_dev;
    libusb_device *new_dev;
    int err, i, j, which_is_new;
    ssize_t current_num_of_devs, new_num_of_devs;
    bool check = 0;

    err = libusb_init(NULL);
    if (err < 0)
        return err;

    current_num_of_devs = libusb_get_device_list(NULL, &current_devs);
    if (current_num_of_devs < 0)
    {
        libusb_exit(NULL);
        return (int) current_num_of_devs;
    }

    while (true)
    {
        i = 0;
        new_num_of_devs = libusb_get_device_list(NULL, &new_devs);
        if (new_num_of_devs < 0)
        {
            libusb_exit(NULL);
            return (int) new_num_of_devs;
        }

        if (new_num_of_devs < current_num_of_devs)
        {
            libusb_free_device_list(current_devs, 1);
            current_num_of_devs = libusb_get_device_list(NULL, &current_devs);
            if (current_num_of_devs < 0)
            {
                libusb_exit(NULL);
                return (int) current_num_of_devs;
            }
            continue;
        }

        while ((new_dev = new_devs[i++]) != NULL)
        {
            struct libusb_device_descriptor new_desc;
            int err = libusb_get_device_descriptor(new_dev, &new_desc);
            if (err < 0)
            {
                fprintf(stderr, "failed to get device descriptor");
                return err;
            }
            j = 0;
            check = 0;
            while ((current_dev = current_devs[j++]) != NULL)
            {
                struct libusb_device_descriptor current_desc;
                int err = libusb_get_device_descriptor(current_dev, &current_desc);
                if (err < 0)
                {
                    fprintf(stderr, "failed to get device descriptor");
                    return err;
                }
                if (new_desc.idProduct == current_desc.idProduct)
                {
                    check = 1;
                    which_is_new = i;
                }

            }
            if (!check)
            {
                system("CLS");
                print_dev(new_devs[which_is_new]);
                libusb_free_device_list(current_devs, 1);
                current_num_of_devs = libusb_get_device_list(NULL, &current_devs);
                if (current_num_of_devs < 0)
                {
                    libusb_exit(NULL);
                    return (int) current_num_of_devs;
                }
                which_is_new = 0;
                break;
            }
        }
        Sleep(1000);
    }

    libusb_exit(NULL);
    return 0;
}
