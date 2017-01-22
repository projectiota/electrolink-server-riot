#!/usr/bin/python
### 
#
# WEIO Web Of Things Platform
# Copyright (C) 2013 Nodesign.net, Uros PETREVSKI, Drasko DRASKOVIC
# All rights reserved
#
#               ##      ## ######## ####  #######  
#               ##  ##  ## ##        ##  ##     ## 
#               ##  ##  ## ##        ##  ##     ## 
#               ##  ##  ## ######    ##  ##     ## 
#               ##  ##  ## ##        ##  ##     ## 
#               ##  ##  ## ##        ##  ##     ## 
#                ###  ###  ######## ####  #######
#
#                    Web Of Things Platform 
#
# This file is part of WEIO and is published under BSD license.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#    This product includes software developed by the WeIO project.
# 4. Neither the name of the WeIO nor the
# names of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY WEIO PROJECT AUTHORS AND CONTRIBUTORS ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL WEIO PROJECT AUTHORS AND CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors : 
# Uros PETREVSKI <uros@nodesign.net>
# Drasko DRASKOVIC <drasko.draskovic@gmail.com>
#
###

import os, glob, urllib, urllib2, time

class Upgrader:

	def __init__(self, reset_pin=22, program_pin=23, fw_file = "/root/Weio_firmware.bin"):
		reset_pin_str = str(reset_pin)
		program_pin_str = str(program_pin)
		self._gpio(reset_pin_str, "/sys/class/gpio/export")
		self._gpio(program_pin_str, "/sys/class/gpio/export")
		self._gpio("out","/sys/class/gpio/gpio"+reset_pin_str+"/direction")
		self._gpio("out","/sys/class/gpio/gpio"+program_pin_str+"/direction")
		self.uper_reset = "/sys/class/gpio/gpio"+reset_pin_str+"/value"
		self.uper_program = "/sys/class/gpio/gpio"+program_pin_str+"/value"
		self.fw_file = fw_file

	def _gpio(self, content, file_name):
		try:
			file_id = os.open(file_name, os.O_WRONLY)
			os.write(file_id, content)
			os.close(file_id)
		except OSError:
			pass

	def _reset_uper(self):
		self._gpio("1",self.uper_reset)
		self._gpio("0",self.uper_reset)

	def upgrade_firmware(self):
		uper_flash_pattern = "CRP DISABLD"

		if not os.path.isfile(self.fw_file):
			# download firmware file
			print "Firmware file not found:", fwUrl
                        sys.exit(1)

		# put  in to programming mode
		self._gpio("1",self.uper_reset)
		self._gpio("1",self.uper_program)
		self._gpio("0",self.uper_reset)
		time.sleep(2) # wait for linux to settle after  reboot in to pgm state
		self._gpio("0",self.uper_program)
		time.sleep(2)

		# find  block device
		list_block_devs = glob.glob("/sys/block/sd*")
		block_device_name = ''
		header = ''
		for try_device_name in list_block_devs:
			try_device_name = "/dev/" + try_device_name.split('/')[-1]
			try:
				block_device = os.open(try_device_name, os.O_RDWR)
				os.lseek(block_device, 3 * 512, os.SEEK_SET)
				header = os.read(block_device,11)
				time.sleep(0.35) # reading can be slowww
				os.close(block_device)
			except OSError:
				print "Error"
			if header == uper_flash_pattern: # "CRP DISABLD"
				print "Found "
				block_device_name = try_device_name # found 
				break;
		if block_device_name == '':
			print " firmware upgrade error, no WeIO was found."
			return

			#os.system("dd if="+fw_file+" of="+new_dev_list[0]+" seek=4")

		# read the fw from file	
		fw_file_id = open(self.fw_file)
		firmware = fw_file_id.read()
		fw_file_id.close()

		block_device = os.open( block_device_name, os.O_RDWR)
		os.lseek(block_device, 4 * 512, os.SEEK_SET)
		os.write(block_device, firmware)
		os.close(block_device)
		# reset 
		self._reset_uper()
		time.sleep(.2)
		return

if __name__ == '__main__':
		#for WeIO up =
		up = Upgrader(17, 22) # GPIO17 =  reset, GPIO22 = WeIO program
		#up = Upgrader()- carambola2 dev board settings
		up.upgrade_firmware()
