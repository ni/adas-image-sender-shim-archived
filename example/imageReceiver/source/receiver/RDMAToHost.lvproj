<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="21008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">false</Property>
	<Property Name="NI.Project.Description" Type="Str">Example Description:
This projects uses three examples to show how to perform continuous high throughput data streaming from disk, to disk, and through memory. This example is compiled for all PXIe-796xR FlexRIO FPGA targets by default. This example is not configured for PXI-795xR FlexRIO targets due to the throughput limitations of the PXI bus.

VI Configuration:
To use this VI with another FPGA target, use the following steps to configure the host VI to use a VI configured for your FPGA target:

1. On the block diagram, right-click the Open FPGA VI Reference (PXI-7966R) VI and select Configure Open FPGA VI Reference.
2. Browse for a new target by clicking the Browse Project button in the Open VI section.
3. Select the VI under the target that matches your device. Click OK to select this VI.
4. Click OK to exit the dialog box and to apply the new selection to the FPGA VI.

Running the VI:
1. Select a RIO Resource that corresponds to the FPGA device configured above.
2. Select a source TDMS file that you will be streaming data to/from.
3. If present in the example, configure the Acquisition Size, Requested host Buffer Depth, Write Size, Read Size, Read/Write Size, Max TDMS Asynchronous Reads, MAX TDMS Asynchronous Writes, and Requested (M) controls on the Host VI Front Panel.
4. Click the run arrow to run the VI.
5. Observe the Status of the Host Buffer as well as the Input and Output DMA FIFOs
6. Observe the achieved Streaming Throughput.
7. Click Stop on the Front Panel to stop the example


Execution Steps:
Refer to the block diagram of the Host VI for each example to view the execution steps for that specific example.</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SubVIs" Type="Folder" URL="../SubVIs">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
		<Item Name="RDMAtoHostDisplay.vi" Type="VI" URL="../RDMAtoHostDisplay.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="RDMA Direction.ctl" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Direction.ctl"/>
				<Item Name="RDMA Connect.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Connect.vi"/>
				<Item Name="Assert Error Cluster Type.vim" Type="VI" URL="/&lt;vilib&gt;/Utility/TypeAssert/Assert Error Cluster Type.vim"/>
				<Item Name="RDMA Create Connector.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Create Connector.vi"/>
				<Item Name="RDMA Connect Connector.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Connect Connector.vi"/>
				<Item Name="RDMA Close.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Close.vi"/>
				<Item Name="RDMA Configure Buffers.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers.vi"/>
				<Item Name="RDMA Configure Buffers U8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers U8.vi"/>
				<Item Name="RDMA Configure Buffers I8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers I8.vi"/>
				<Item Name="RDMA Configure Buffers U16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers U16.vi"/>
				<Item Name="RDMA Configure Buffers I16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers I16.vi"/>
				<Item Name="RDMA Configure Buffers U32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers U32.vi"/>
				<Item Name="RDMA Configure Buffers I32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers I32.vi"/>
				<Item Name="RDMA Configure Buffers U64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers U64.vi"/>
				<Item Name="RDMA Configure Buffers I64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers I64.vi"/>
				<Item Name="RDMA Configure Buffers DBL.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Configure Buffers DBL.vi"/>
				<Item Name="RDMA Acquire Received Buffer.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer.vi"/>
				<Item Name="RDMA Acquire Received Buffer U8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer U8.vi"/>
				<Item Name="RDMA Acquire Received Buffer I8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer I8.vi"/>
				<Item Name="RDMA Acquire Received Buffer U16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer U16.vi"/>
				<Item Name="RDMA Acquire Received Buffer I16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer I16.vi"/>
				<Item Name="RDMA Acquire Received Buffer U32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer U32.vi"/>
				<Item Name="RDMA Acquire Received Buffer I32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer I32.vi"/>
				<Item Name="RDMA Acquire Received Buffer U64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer U64.vi"/>
				<Item Name="RDMA Acquire Received Buffer I64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer I64.vi"/>
				<Item Name="RDMA Acquire Received Buffer DBL.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Received Buffer DBL.vi"/>
				<Item Name="IMAQ Image.ctl" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/IMAQ Image.ctl"/>
				<Item Name="Image Type" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/Image Type"/>
				<Item Name="IMAQ Create" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Create"/>
				<Item Name="IMAQ Remote Display Options" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Remote Display Options"/>
				<Item Name="High Resolution Relative Seconds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/High Resolution Relative Seconds.vi"/>
				<Item Name="RDMA Set Used Send Buffer Size.vim" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Set Used Send Buffer Size.vim"/>
				<Item Name="Assert Unsigned Integer Type.vim" Type="VI" URL="/&lt;vilib&gt;/Utility/TypeAssert/Assert Unsigned Integer Type.vim"/>
				<Item Name="RDMA Acquire Send Buffer.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer.vi"/>
				<Item Name="RDMA Acquire Send Buffer U8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer U8.vi"/>
				<Item Name="RDMA Acquire Send Buffer DBL.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer DBL.vi"/>
				<Item Name="RDMA Acquire Send Buffer I8.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer I8.vi"/>
				<Item Name="RDMA Acquire Send Buffer I16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer I16.vi"/>
				<Item Name="RDMA Acquire Send Buffer I32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer I32.vi"/>
				<Item Name="RDMA Acquire Send Buffer I64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer I64.vi"/>
				<Item Name="RDMA Acquire Send Buffer U16.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer U16.vi"/>
				<Item Name="RDMA Acquire Send Buffer U32.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer U32.vi"/>
				<Item Name="RDMA Acquire Send Buffer U64.vi" Type="VI" URL="/&lt;vilib&gt;/rdma/rdma.llb/RDMA Acquire Send Buffer U64.vi"/>
			</Item>
			<Item Name="nirdma.dll" Type="Document" URL="nirdma.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="PixelFormat.ctl" Type="VI" URL="../../../utility/decodeToImage/PixelFormat.ctl"/>
			<Item Name="DecodeImageArray.vi" Type="VI" URL="../../../utility/decodeToImage/DecodeImageArray.vi"/>
			<Item Name="nivissvc.dll" Type="Document" URL="nivissvc.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="ConfigureBackChannel.vi" Type="VI" URL="../ConfigureBackChannel.vi"/>
			<Item Name="SendBackChannelTrigger.vi" Type="VI" URL="../SendBackChannelTrigger.vi"/>
			<Item Name="SerializeEmbeddedDataLines.vi" Type="VI" URL="../SerializeEmbeddedDataLines.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
