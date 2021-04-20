#
# OMNeT++/OMNEST Makefile for $(LIB_PREFIX)INET
#
# This file was generated with the command:
#  opp_makemake --make-so -f --deep -o INET -O out -pINET -I. -Xinet/applications/dhcp -Xinet/applications/ethernet -Xinet/applications/httptools -Xinet/applications/packetdrill -Xinet/applications/rtpapp -Xinet/applications/sctpapp -Xinet/applications/tcpapp -Xinet/applications/tunapp -Xinet/applications/voip -Xinet/applications/voipstream -Xinet/emulation -Xinet/linklayer/bmac -Xinet/linklayer/ethernet -Xinet/linklayer/ieee8021d -Xinet/linklayer/ieee8021q -Xinet/linklayer/lmac -Xinet/linklayer/ppp -Xinet/linklayer/tun -Xinet/linklayer/vlan -Xinet/linklayer/xmac -Xinet/networklayer/configurator/ipv6 -Xinet/networklayer/diffserv -Xinet/networklayer/icmpv6 -Xinet/networklayer/internetcloud -Xinet/networklayer/ipv6 -Xinet/networklayer/ipv6tunneling -Xinet/networklayer/ldp -Xinet/networklayer/mpls -Xinet/networklayer/rsvpte -Xinet/networklayer/ted -Xinet/networklayer/xmipv6 -Xinet/node/aodv -Xinet/node/bgp -Xinet/node/dymo -Xinet/node/ethernet -Xinet/node/gpsr -Xinet/node/httptools -Xinet/node/internetcloud -Xinet/node/ipv6 -Xinet/node/mpls -Xinet/node/ospfv2 -Xinet/node/ospfv3 -Xinet/node/packetdrill -Xinet/node/rip -Xinet/node/rtp -Xinet/node/xmipv6 -Xinet/physicallayer/ethernet -Xinet/routing/aodv -Xinet/routing/bgpv4 -Xinet/routing/dymo -Xinet/routing/gpsr -Xinet/routing/ospfv2 -Xinet/routing/ospfv3 -Xinet/routing/pim -Xinet/routing/rip -Xinet/transportlayer/rtp -Xinet/transportlayer/sctp -Xinet/transportlayer/tcp -Xinet/transportlayer/tcp_common -Xinet/transportlayer/tcp_lwip -Xinet/transportlayer/tcp_nsc
#

# Name of target to be created (-o option)
TARGET = $(LIB_PREFIX)INET$(D)$(SHARED_LIB_SUFFIX)
TARGET_DIR = .

# C++ include paths (with -I)
INCLUDE_PATH = -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/inet/applications/Fap/Fap.o \
    $O/inet/applications/Fap/hash/MurmurHash3.o \
    $O/inet/applications/ICNapp/SimpleApp.o \
    $O/inet/applications/ICNapp/SimpleProvider.o \
    $O/inet/applications/base/ApplicationBase.o \
    $O/inet/applications/base/ApplicationPacketSerializer.o \
    $O/inet/applications/generic/IpvxTrafGen.o \
    $O/inet/applications/generic/IpvxTrafSink.o \
    $O/inet/applications/netperfmeter/NetPerfMeter.o \
    $O/inet/applications/pccpapp/PccpAlg.o \
    $O/inet/applications/pccpapp/PccpApp.o \
    $O/inet/applications/pccpapp/PccpBurstApp.o \
    $O/inet/applications/pccpapp/PccpIoTApp.o \
    $O/inet/applications/pccpapp/PccpProvider.o \
    $O/inet/applications/pccpapp/PccpSendQueue.o \
    $O/inet/applications/pccpapp/PccpStateVariables.o \
    $O/inet/applications/pingapp/PingApp.o \
    $O/inet/applications/udpapp/UdpBasicApp.o \
    $O/inet/applications/udpapp/UdpBasicBurst.o \
    $O/inet/applications/udpapp/UdpEchoApp.o \
    $O/inet/applications/udpapp/UdpSink.o \
    $O/inet/applications/udpapp/UdpSocketIo.o \
    $O/inet/applications/udpapp/UdpVideoStreamClient.o \
    $O/inet/applications/udpapp/UdpVideoStreamServer.o \
    $O/inet/common/BitVector.o \
    $O/inet/common/Compat.o \
    $O/inet/common/IInterfaceRegistrationListener.o \
    $O/inet/common/INETUtils.o \
    $O/inet/common/InitStages.o \
    $O/inet/common/int128.o \
    $O/inet/common/IntervalTree.o \
    $O/inet/common/IProtocolRegistrationListener.o \
    $O/inet/common/LayeredProtocolBase.o \
    $O/inet/common/Macho.o \
    $O/inet/common/MatchableField.o \
    $O/inet/common/MatchableObject.o \
    $O/inet/common/MemoryInputStream.o \
    $O/inet/common/MemoryOutputStream.o \
    $O/inet/common/MessageDispatcher.o \
    $O/inet/common/ModuleAccess.o \
    $O/inet/common/NedFunctions.o \
    $O/inet/common/NetworkCommunicationFingerprintCalculator.o \
    $O/inet/common/NetworkNamespaceContext.o \
    $O/inet/common/ObjectPrinter.o \
    $O/inet/common/OsgScene.o \
    $O/inet/common/OsgUtils.o \
    $O/inet/common/PatternMatcher.o \
    $O/inet/common/Protocol.o \
    $O/inet/common/ProtocolGroup.o \
    $O/inet/common/ProtocolTools.o \
    $O/inet/common/ResultFilters.o \
    $O/inet/common/ResultRecorders.o \
    $O/inet/common/ShortBitVector.o \
    $O/inet/common/Simsignals.o \
    $O/inet/common/StringFormat.o \
    $O/inet/common/TlvOptions.o \
    $O/inet/common/Topology.o \
    $O/inet/common/XMLUtils.o \
    $O/inet/common/bloomfilter/hash/MurmurHash3.o \
    $O/inet/common/checksum/EthernetCRC.o \
    $O/inet/common/checksum/TcpIpChecksum.o \
    $O/inet/common/figures/BarFigure.o \
    $O/inet/common/figures/BoxedLabelFigure.o \
    $O/inet/common/figures/CounterFigure.o \
    $O/inet/common/figures/FigureRecorder.o \
    $O/inet/common/figures/GaugeFigure.o \
    $O/inet/common/figures/HeatMapFigure.o \
    $O/inet/common/figures/HeatMapPlotFigure.o \
    $O/inet/common/figures/IndexedImageFigure.o \
    $O/inet/common/figures/IndicatorLabelFigure.o \
    $O/inet/common/figures/IndicatorTextFigure.o \
    $O/inet/common/figures/InstrumentUtil.o \
    $O/inet/common/figures/LabeledIconFigure.o \
    $O/inet/common/figures/LabeledLineFigure.o \
    $O/inet/common/figures/LabeledPolylineFigure.o \
    $O/inet/common/figures/LinearGaugeFigure.o \
    $O/inet/common/figures/PlotFigure.o \
    $O/inet/common/figures/ProgressMeterFigure.o \
    $O/inet/common/figures/QueueFigure.o \
    $O/inet/common/figures/SignalFigure.o \
    $O/inet/common/figures/SimTimeTextFigure.o \
    $O/inet/common/figures/ThermometerFigure.o \
    $O/inet/common/figures/TrailFigure.o \
    $O/inet/common/geometry/base/GeometricObjectBase.o \
    $O/inet/common/geometry/base/ShapeBase.o \
    $O/inet/common/geometry/common/CanvasProjection.o \
    $O/inet/common/geometry/common/Coord.o \
    $O/inet/common/geometry/common/EulerAngles.o \
    $O/inet/common/geometry/common/GeographicCoordinateSystem.o \
    $O/inet/common/geometry/common/Quaternion.o \
    $O/inet/common/geometry/common/RotationMatrix.o \
    $O/inet/common/geometry/container/BvhTree.o \
    $O/inet/common/geometry/container/QuadTree.o \
    $O/inet/common/geometry/container/SpatialGrid.o \
    $O/inet/common/geometry/object/Box.o \
    $O/inet/common/geometry/object/LineSegment.o \
    $O/inet/common/geometry/object/Plane.o \
    $O/inet/common/geometry/object/Polygon.o \
    $O/inet/common/geometry/shape/Cuboid.o \
    $O/inet/common/geometry/shape/Prism.o \
    $O/inet/common/geometry/shape/Sphere.o \
    $O/inet/common/geometry/shape/polyhedron/Polyhedron.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronEdge.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronFace.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronPoint.o \
    $O/inet/common/lifecycle/ApplicationOperations.o \
    $O/inet/common/lifecycle/InterfaceOperations.o \
    $O/inet/common/lifecycle/LifecycleController.o \
    $O/inet/common/lifecycle/ModuleOperations.o \
    $O/inet/common/lifecycle/NodeStatus.o \
    $O/inet/common/lifecycle/OperationalBase.o \
    $O/inet/common/misc/MessageChecker.o \
    $O/inet/common/misc/NetAnimTrace.o \
    $O/inet/common/misc/SignalSource.o \
    $O/inet/common/misc/ThruputMeter.o \
    $O/inet/common/misc/ThruputMeteringChannel.o \
    $O/inet/common/packet/ChunkBuffer.o \
    $O/inet/common/packet/ChunkQueue.o \
    $O/inet/common/packet/Message.o \
    $O/inet/common/packet/Packet.o \
    $O/inet/common/packet/PacketFilter.o \
    $O/inet/common/packet/ReassemblyBuffer.o \
    $O/inet/common/packet/ReorderBuffer.o \
    $O/inet/common/packet/chunk/BitCountChunk.o \
    $O/inet/common/packet/chunk/BitsChunk.o \
    $O/inet/common/packet/chunk/ByteCountChunk.o \
    $O/inet/common/packet/chunk/BytesChunk.o \
    $O/inet/common/packet/chunk/Chunk.o \
    $O/inet/common/packet/chunk/cPacketChunk.o \
    $O/inet/common/packet/chunk/EmptyChunk.o \
    $O/inet/common/packet/chunk/FieldsChunk.o \
    $O/inet/common/packet/chunk/SequenceChunk.o \
    $O/inet/common/packet/chunk/SliceChunk.o \
    $O/inet/common/packet/dissector/PacketDissector.o \
    $O/inet/common/packet/dissector/ProtocolDissector.o \
    $O/inet/common/packet/dissector/ProtocolDissectorRegistry.o \
    $O/inet/common/packet/printer/PacketPrinter.o \
    $O/inet/common/packet/printer/ProtocolPrinter.o \
    $O/inet/common/packet/printer/ProtocolPrinterRegistry.o \
    $O/inet/common/packet/recorder/PacketDump.o \
    $O/inet/common/packet/recorder/PcapReader.o \
    $O/inet/common/packet/recorder/PcapRecorder.o \
    $O/inet/common/packet/recorder/PcapWriter.o \
    $O/inet/common/packet/serializer/BitCountChunkSerializer.o \
    $O/inet/common/packet/serializer/BitsChunkSerializer.o \
    $O/inet/common/packet/serializer/ByteCountChunkSerializer.o \
    $O/inet/common/packet/serializer/BytesChunkSerializer.o \
    $O/inet/common/packet/serializer/ChunkSerializer.o \
    $O/inet/common/packet/serializer/ChunkSerializerRegistry.o \
    $O/inet/common/packet/serializer/EmptyChunkSerializer.o \
    $O/inet/common/packet/serializer/FieldsChunkSerializer.o \
    $O/inet/common/packet/serializer/SequenceChunkSerializer.o \
    $O/inet/common/packet/serializer/SliceChunkSerializer.o \
    $O/inet/common/packet/tag/RegionTagSet.o \
    $O/inet/common/packet/tag/TagSet.o \
    $O/inet/common/scenario/ScenarioManager.o \
    $O/inet/common/scheduler/RealTimeScheduler.o \
    $O/inet/common/socket/SocketMap.o \
    $O/inet/common/streetmap/OpenStreetMap.o \
    $O/inet/environment/common/Material.o \
    $O/inet/environment/common/MaterialRegistry.o \
    $O/inet/environment/common/PhysicalEnvironment.o \
    $O/inet/environment/common/PhysicalObject.o \
    $O/inet/environment/ground/FlatGround.o \
    $O/inet/environment/ground/OsgEarthGround.o \
    $O/inet/environment/objectcache/BvhObjectCache.o \
    $O/inet/environment/objectcache/GridObjectCache.o \
    $O/inet/linklayer/acking/AckingMac.o \
    $O/inet/linklayer/acking/AckingMacHeaderSerializer.o \
    $O/inet/linklayer/acking/AckingMacProtocolDissector.o \
    $O/inet/linklayer/acking/AckingMacProtocolPrinter.o \
    $O/inet/linklayer/base/MacProtocolBase.o \
    $O/inet/linklayer/common/ExampleQosClassifier.o \
    $O/inet/linklayer/common/FcsMode.o \
    $O/inet/linklayer/common/MacAddress.o \
    $O/inet/linklayer/common/MacAddressType.o \
    $O/inet/linklayer/common/QosClassifier.o \
    $O/inet/linklayer/common/RandomQosClassifier.o \
    $O/inet/linklayer/common/UserPriorityPacketComparatorFunction.o \
    $O/inet/linklayer/configurator/Ieee8021dInterfaceData.o \
    $O/inet/linklayer/configurator/L2NetworkConfigurator.o \
    $O/inet/linklayer/configurator/L2NodeConfigurator.o \
    $O/inet/linklayer/csmaca/CsmaCaMac.o \
    $O/inet/linklayer/csmaca/CsmaCaMacHeaderSerializer.o \
    $O/inet/linklayer/csmaca/CsmaCaMacProtocolDissector.o \
    $O/inet/linklayer/csmaca/CsmaCaMacProtocolPrinter.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211EtherTypeHeaderSerializer.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211EtherTypeProtocolDissector.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211EtherTypeProtocolPrinter.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211LlcEpd.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211LlcLpd.o \
    $O/inet/linklayer/ieee80211/mac/Ds.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211Mac.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211MacHeaderSerializer.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211MacProtocolDissector.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211MacProtocolPrinter.o \
    $O/inet/linklayer/ieee80211/mac/Rx.o \
    $O/inet/linklayer/ieee80211/mac/Tx.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/BasicMpduAggregationPolicy.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/BasicMsduAggregationPolicy.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/MpduAggregation.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/MpduDeaggregation.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/MsduAggregation.o \
    $O/inet/linklayer/ieee80211/mac/aggregation/MsduDeaggregation.o \
    $O/inet/linklayer/ieee80211/mac/blockack/BlockAckRecord.o \
    $O/inet/linklayer/ieee80211/mac/blockack/OriginatorBlockAckAgreementHandler.o \
    $O/inet/linklayer/ieee80211/mac/blockack/OriginatorBlockAckAgreementPolicy.o \
    $O/inet/linklayer/ieee80211/mac/blockack/OriginatorBlockAckProcedure.o \
    $O/inet/linklayer/ieee80211/mac/blockack/RecipientBlockAckAgreement.o \
    $O/inet/linklayer/ieee80211/mac/blockack/RecipientBlockAckAgreementHandler.o \
    $O/inet/linklayer/ieee80211/mac/blockack/RecipientBlockAckAgreementPolicy.o \
    $O/inet/linklayer/ieee80211/mac/blockack/RecipientBlockAckProcedure.o \
    $O/inet/linklayer/ieee80211/mac/blockackreordering/BlockAckReordering.o \
    $O/inet/linklayer/ieee80211/mac/blockackreordering/ReceiveBuffer.o \
    $O/inet/linklayer/ieee80211/mac/channelaccess/Dcaf.o \
    $O/inet/linklayer/ieee80211/mac/channelaccess/Edca.o \
    $O/inet/linklayer/ieee80211/mac/channelaccess/Edcaf.o \
    $O/inet/linklayer/ieee80211/mac/channelaccess/Hcca.o \
    $O/inet/linklayer/ieee80211/mac/common/ModeSetListener.o \
    $O/inet/linklayer/ieee80211/mac/common/ResultFilters.o \
    $O/inet/linklayer/ieee80211/mac/common/SequenceControlField.o \
    $O/inet/linklayer/ieee80211/mac/contention/Contention.o \
    $O/inet/linklayer/ieee80211/mac/contention/EdcaCollisionController.o \
    $O/inet/linklayer/ieee80211/mac/contract/IChannelAccess.o \
    $O/inet/linklayer/ieee80211/mac/contract/IContention.o \
    $O/inet/linklayer/ieee80211/mac/contract/IFrameSequenceHandler.o \
    $O/inet/linklayer/ieee80211/mac/contract/IOriginatorMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/contract/IRateSelection.o \
    $O/inet/linklayer/ieee80211/mac/contract/IRecipientMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/contract/IRecipientQosMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/contract/IRecoveryProcedure.o \
    $O/inet/linklayer/ieee80211/mac/coordinationfunction/Dcf.o \
    $O/inet/linklayer/ieee80211/mac/coordinationfunction/Hcf.o \
    $O/inet/linklayer/ieee80211/mac/coordinationfunction/Mcf.o \
    $O/inet/linklayer/ieee80211/mac/coordinationfunction/Pcf.o \
    $O/inet/linklayer/ieee80211/mac/duplicateremoval/LegacyDuplicateRemoval.o \
    $O/inet/linklayer/ieee80211/mac/duplicateremoval/QosDuplicateRemoval.o \
    $O/inet/linklayer/ieee80211/mac/fragmentation/BasicFragmentationPolicy.o \
    $O/inet/linklayer/ieee80211/mac/fragmentation/BasicReassembly.o \
    $O/inet/linklayer/ieee80211/mac/fragmentation/Defragmentation.o \
    $O/inet/linklayer/ieee80211/mac/fragmentation/Fragmentation.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/DcfFs.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/FrameSequenceContext.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/FrameSequenceHandler.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/GenericFrameSequences.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/HcfFs.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/HtTxOpFs.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/McfFs.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/PcfFs.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/PrimitiveFrameSequences.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/TxOpFs.o \
    $O/inet/linklayer/ieee80211/mac/lifetime/DcfReceiveLifetimeHandler.o \
    $O/inet/linklayer/ieee80211/mac/lifetime/DcfTransmitLifetimeHandler.o \
    $O/inet/linklayer/ieee80211/mac/lifetime/EdcaTransmitLifetimeHandler.o \
    $O/inet/linklayer/ieee80211/mac/originator/AckHandler.o \
    $O/inet/linklayer/ieee80211/mac/originator/NonQosRecoveryProcedure.o \
    $O/inet/linklayer/ieee80211/mac/originator/OriginatorAckPolicy.o \
    $O/inet/linklayer/ieee80211/mac/originator/OriginatorMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/originator/OriginatorQosAckPolicy.o \
    $O/inet/linklayer/ieee80211/mac/originator/OriginatorQosMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/originator/QosAckHandler.o \
    $O/inet/linklayer/ieee80211/mac/originator/QosRecoveryProcedure.o \
    $O/inet/linklayer/ieee80211/mac/originator/QosRtsPolicy.o \
    $O/inet/linklayer/ieee80211/mac/originator/RtsPolicy.o \
    $O/inet/linklayer/ieee80211/mac/originator/RtsProcedure.o \
    $O/inet/linklayer/ieee80211/mac/originator/TxopProcedure.o \
    $O/inet/linklayer/ieee80211/mac/protectionmechanism/OriginatorProtectionMechanism.o \
    $O/inet/linklayer/ieee80211/mac/protectionmechanism/SingleProtectionMechanism.o \
    $O/inet/linklayer/ieee80211/mac/queue/InProgressFrames.o \
    $O/inet/linklayer/ieee80211/mac/queue/PendingQueue.o \
    $O/inet/linklayer/ieee80211/mac/ratecontrol/AarfRateControl.o \
    $O/inet/linklayer/ieee80211/mac/ratecontrol/OnoeRateControl.o \
    $O/inet/linklayer/ieee80211/mac/ratecontrol/RateControlBase.o \
    $O/inet/linklayer/ieee80211/mac/rateselection/QosRateSelection.o \
    $O/inet/linklayer/ieee80211/mac/rateselection/RateSelection.o \
    $O/inet/linklayer/ieee80211/mac/recipient/CtsPolicy.o \
    $O/inet/linklayer/ieee80211/mac/recipient/CtsProcedure.o \
    $O/inet/linklayer/ieee80211/mac/recipient/QosCtsPolicy.o \
    $O/inet/linklayer/ieee80211/mac/recipient/RecipientAckPolicy.o \
    $O/inet/linklayer/ieee80211/mac/recipient/RecipientAckProcedure.o \
    $O/inet/linklayer/ieee80211/mac/recipient/RecipientMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/recipient/RecipientQosAckPolicy.o \
    $O/inet/linklayer/ieee80211/mac/recipient/RecipientQosMacDataService.o \
    $O/inet/linklayer/ieee80211/mac/sequencenumberassignment/LegacySequenceNumberAssigment.o \
    $O/inet/linklayer/ieee80211/mac/sequencenumberassignment/NonQoSSequenceNumberAssignment.o \
    $O/inet/linklayer/ieee80211/mac/sequencenumberassignment/QoSSequenceNumberAssignment.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211AgentSta.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhoc.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAp.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtApBase.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtApSimplified.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtFrameSerializer.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtProtocolDissector.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtProtocolPrinter.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtSta.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtStaSimplified.o \
    $O/inet/linklayer/ieee80211/mib/Ieee80211Mib.o \
    $O/inet/linklayer/ieee80211/portal/Ieee80211Portal.o \
    $O/inet/linklayer/ieee802154/Ieee802154Mac.o \
    $O/inet/linklayer/ieee802154/Ieee802154ProtocolDissector.o \
    $O/inet/linklayer/ieee8022/Ieee8022Llc.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcHeaderSerializer.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcProtocolDissector.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcProtocolPrinter.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcSocket.o \
    $O/inet/linklayer/loopback/Loopback.o \
    $O/inet/linklayer/shortcut/ShortcutMac.o \
    $O/inet/linklayer/shortcut/ShortcutMacProtocolDissector.o \
    $O/inet/mobility/base/LinearRotatingMobilityBase.o \
    $O/inet/mobility/base/LineSegmentsMobilityBase.o \
    $O/inet/mobility/base/MobilityBase.o \
    $O/inet/mobility/base/MovingMobilityBase.o \
    $O/inet/mobility/base/RotatingMobilityBase.o \
    $O/inet/mobility/contract/IMobility.o \
    $O/inet/mobility/droneGroup/AttachedTriangle.o \
    $O/inet/mobility/droneGroup/TriangleLinearMobility.o \
    $O/inet/mobility/group/MoBanCoordinator.o \
    $O/inet/mobility/group/MoBanLocal.o \
    $O/inet/mobility/group/Posture.o \
    $O/inet/mobility/group/PostureTransition.o \
    $O/inet/mobility/single/AnsimMobility.o \
    $O/inet/mobility/single/AttachedMobility.o \
    $O/inet/mobility/single/BonnMotionFileCache.o \
    $O/inet/mobility/single/BonnMotionMobility.o \
    $O/inet/mobility/single/ChiangMobility.o \
    $O/inet/mobility/single/CircleMobility.o \
    $O/inet/mobility/single/FacingMobility.o \
    $O/inet/mobility/single/GaussMarkovMobility.o \
    $O/inet/mobility/single/LinearMobility.o \
    $O/inet/mobility/single/MassMobility.o \
    $O/inet/mobility/single/Ns2MotionMobility.o \
    $O/inet/mobility/single/RandomWaypointMobility.o \
    $O/inet/mobility/single/RectangleMobility.o \
    $O/inet/mobility/single/SuperpositioningMobility.o \
    $O/inet/mobility/single/TractorMobility.o \
    $O/inet/mobility/single/TurtleMobility.o \
    $O/inet/mobility/single/VehicleMobility.o \
    $O/inet/mobility/static/StaticConcentricMobility.o \
    $O/inet/mobility/static/StaticGridMobility.o \
    $O/inet/mobility/static/StaticLinearMobility.o \
    $O/inet/mobility/static/StaticTriMobility.o \
    $O/inet/mobility/static/StationaryMobility.o \
    $O/inet/networklayer/arp/ipv4/Arp.o \
    $O/inet/networklayer/arp/ipv4/ArpPacket.o \
    $O/inet/networklayer/arp/ipv4/ArpPacketSerializer.o \
    $O/inet/networklayer/arp/ipv4/ArpProtocolDissector.o \
    $O/inet/networklayer/arp/ipv4/ArpProtocolPrinter.o \
    $O/inet/networklayer/arp/ipv4/GlobalArp.o \
    $O/inet/networklayer/base/NetworkProtocolBase.o \
    $O/inet/networklayer/common/EchoPacketSerializer.o \
    $O/inet/networklayer/common/EchoProtocol.o \
    $O/inet/networklayer/common/EchoProtocolDissector.o \
    $O/inet/networklayer/common/EchoProtocolPrinter.o \
    $O/inet/networklayer/common/InterfaceEntry.o \
    $O/inet/networklayer/common/InterfaceMatcher.o \
    $O/inet/networklayer/common/InterfaceTable.o \
    $O/inet/networklayer/common/L3Address.o \
    $O/inet/networklayer/common/L3AddressResolver.o \
    $O/inet/networklayer/common/L3Tools.o \
    $O/inet/networklayer/common/ModuleIdAddress.o \
    $O/inet/networklayer/common/ModuleIdAddressType.o \
    $O/inet/networklayer/common/ModulePathAddress.o \
    $O/inet/networklayer/common/ModulePathAddressType.o \
    $O/inet/networklayer/common/NetfilterInfoHook.o \
    $O/inet/networklayer/configurator/base/NetworkConfiguratorBase.o \
    $O/inet/networklayer/configurator/ipv4/HostAutoConfigurator.o \
    $O/inet/networklayer/configurator/ipv4/Ipv4FlatNetworkConfigurator.o \
    $O/inet/networklayer/configurator/ipv4/Ipv4NetworkConfigurator.o \
    $O/inet/networklayer/configurator/ipv4/Ipv4NodeConfigurator.o \
    $O/inet/networklayer/configurator/nexthop/NextHopNetworkConfigurator.o \
    $O/inet/networklayer/contract/IArp.o \
    $O/inet/networklayer/contract/IRoute.o \
    $O/inet/networklayer/contract/L3Socket.o \
    $O/inet/networklayer/contract/NetworkHeaderBase.o \
    $O/inet/networklayer/contract/clns/ClnsAddress.o \
    $O/inet/networklayer/contract/clns/ClnsAddressType.o \
    $O/inet/networklayer/contract/color/ColorSocket.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Address.o \
    $O/inet/networklayer/contract/ipv4/Ipv4AddressType.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Socket.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Address.o \
    $O/inet/networklayer/contract/ipv6/Ipv6AddressType.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Socket.o \
    $O/inet/networklayer/flooding/Flooding.o \
    $O/inet/networklayer/flooding/FloodingProtocolDissector.o \
    $O/inet/networklayer/flooding/FloodingProtocolPrinter.o \
    $O/inet/networklayer/icn/cacheTable/colorCacheTable.o \
    $O/inet/networklayer/icn/cacheTable/colorChunk.o \
    $O/inet/networklayer/icn/cacheTable/contentBlock.o \
    $O/inet/networklayer/icn/cluster/SimpleCluster.o \
    $O/inet/networklayer/icn/cluster/TwoNicCluster.o \
    $O/inet/networklayer/icn/color/color.o \
    $O/inet/networklayer/icn/color/ColorFragBuf.o \
    $O/inet/networklayer/icn/color/ColorNoCluster.o \
    $O/inet/networklayer/icn/color/colorTwoInterface.o \
    $O/inet/networklayer/icn/delayQueue/delayQueue.o \
    $O/inet/networklayer/icn/field/NID.o \
    $O/inet/networklayer/icn/field/SID.o \
    $O/inet/networklayer/icn/pendingTable/colorPendingGetTable.o \
    $O/inet/networklayer/icn/routingTable/colorRoutingTable.o \
    $O/inet/networklayer/ipv4/Icmp.o \
    $O/inet/networklayer/ipv4/IcmpHeaderSerializer.o \
    $O/inet/networklayer/ipv4/IcmpProtocolDissector.o \
    $O/inet/networklayer/ipv4/IcmpProtocolPrinter.o \
    $O/inet/networklayer/ipv4/IgmpHeaderSerializer.o \
    $O/inet/networklayer/ipv4/IgmpProtocolDissector.o \
    $O/inet/networklayer/ipv4/IgmpProtocolPrinter.o \
    $O/inet/networklayer/ipv4/Igmpv2.o \
    $O/inet/networklayer/ipv4/Igmpv3.o \
    $O/inet/networklayer/ipv4/Ipv4.o \
    $O/inet/networklayer/ipv4/Ipv4FragBuf.o \
    $O/inet/networklayer/ipv4/Ipv4Header.o \
    $O/inet/networklayer/ipv4/Ipv4HeaderSerializer.o \
    $O/inet/networklayer/ipv4/Ipv4InterfaceData.o \
    $O/inet/networklayer/ipv4/Ipv4NatTable.o \
    $O/inet/networklayer/ipv4/Ipv4NetworkLayer.o \
    $O/inet/networklayer/ipv4/Ipv4ProtocolDissector.o \
    $O/inet/networklayer/ipv4/Ipv4ProtocolPrinter.o \
    $O/inet/networklayer/ipv4/Ipv4Route.o \
    $O/inet/networklayer/ipv4/Ipv4RoutingTable.o \
    $O/inet/networklayer/ipv4/NetworkInfo.o \
    $O/inet/networklayer/ipv4/RoutingTableParser.o \
    $O/inet/networklayer/ipv4/RoutingTableRecorder.o \
    $O/inet/networklayer/nexthop/NextHopForwarding.o \
    $O/inet/networklayer/nexthop/NextHopInterfaceData.o \
    $O/inet/networklayer/nexthop/NextHopProtocolDissector.o \
    $O/inet/networklayer/nexthop/NextHopProtocolPrinter.o \
    $O/inet/networklayer/nexthop/NextHopRoute.o \
    $O/inet/networklayer/nexthop/NextHopRoutingTable.o \
    $O/inet/networklayer/probabilistic/AdaptiveProbabilisticBroadcast.o \
    $O/inet/networklayer/probabilistic/ProbabilisticBroadcast.o \
    $O/inet/networklayer/probabilistic/ProbabilisticProtocolDissector.o \
    $O/inet/networklayer/probabilistic/ProbabilisticProtocolPrinter.o \
    $O/inet/networklayer/wiseroute/WiseRoute.o \
    $O/inet/networklayer/wiseroute/WiseRouteProtocolDissector.o \
    $O/inet/networklayer/wiseroute/WiseRouteProtocolPrinter.o \
    $O/inet/physicallayer/analogmodel/bitlevel/DimensionalSignalAnalogModel.o \
    $O/inet/physicallayer/analogmodel/bitlevel/LayeredDimensionalAnalogModel.o \
    $O/inet/physicallayer/analogmodel/bitlevel/LayeredScalarAnalogModel.o \
    $O/inet/physicallayer/analogmodel/bitlevel/ScalarSignalAnalogModel.o \
    $O/inet/physicallayer/analogmodel/bitlevel/SignalAnalogModel.o \
    $O/inet/physicallayer/analogmodel/packetlevel/DimensionalAnalogModel.o \
    $O/inet/physicallayer/analogmodel/packetlevel/DimensionalNoise.o \
    $O/inet/physicallayer/analogmodel/packetlevel/DimensionalReception.o \
    $O/inet/physicallayer/analogmodel/packetlevel/DimensionalSnir.o \
    $O/inet/physicallayer/analogmodel/packetlevel/DimensionalTransmission.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarAnalogModel.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarNoise.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarReception.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarSnir.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarTransmission.o \
    $O/inet/physicallayer/antenna/AxiallySymmetricAntenna.o \
    $O/inet/physicallayer/antenna/ConstantGainAntenna.o \
    $O/inet/physicallayer/antenna/CosineAntenna.o \
    $O/inet/physicallayer/antenna/DipoleAntenna.o \
    $O/inet/physicallayer/antenna/InterpolatingAntenna.o \
    $O/inet/physicallayer/antenna/IsotropicAntenna.o \
    $O/inet/physicallayer/antenna/ParabolicAntenna.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskCode.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskDecoder.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskDemodulator.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskEncoder.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskLayeredReceiver.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskLayeredTransmitter.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskModulator.o \
    $O/inet/physicallayer/apskradio/bitlevel/ApskPhyHeaderSerializer.o \
    $O/inet/physicallayer/apskradio/bitlevel/errormodel/ApskLayeredErrorModel.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskDimensionalReceiver.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskDimensionalTransmission.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskDimensionalTransmitter.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskProtocolDissector.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskRadio.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskScalarReceiver.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskScalarTransmission.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskScalarTransmitter.o \
    $O/inet/physicallayer/apskradio/packetlevel/errormodel/ApskErrorModel.o \
    $O/inet/physicallayer/backgroundnoise/DimensionalBackgroundNoise.o \
    $O/inet/physicallayer/backgroundnoise/IsotropicDimensionalBackgroundNoise.o \
    $O/inet/physicallayer/backgroundnoise/IsotropicScalarBackgroundNoise.o \
    $O/inet/physicallayer/base/bitlevel/LayeredErrorModelBase.o \
    $O/inet/physicallayer/base/packetlevel/AnalogModelBase.o \
    $O/inet/physicallayer/base/packetlevel/AntennaBase.o \
    $O/inet/physicallayer/base/packetlevel/ApskModulationBase.o \
    $O/inet/physicallayer/base/packetlevel/CommunicationCacheBase.o \
    $O/inet/physicallayer/base/packetlevel/DimensionalAnalogModelBase.o \
    $O/inet/physicallayer/base/packetlevel/DimensionalTransmitterBase.o \
    $O/inet/physicallayer/base/packetlevel/DpskModulationBase.o \
    $O/inet/physicallayer/base/packetlevel/ErrorModelBase.o \
    $O/inet/physicallayer/base/packetlevel/FlatRadioBase.o \
    $O/inet/physicallayer/base/packetlevel/FlatReceiverBase.o \
    $O/inet/physicallayer/base/packetlevel/FlatReceptionBase.o \
    $O/inet/physicallayer/base/packetlevel/FlatTransmissionBase.o \
    $O/inet/physicallayer/base/packetlevel/FlatTransmitterBase.o \
    $O/inet/physicallayer/base/packetlevel/GfskModulationBase.o \
    $O/inet/physicallayer/base/packetlevel/ListeningBase.o \
    $O/inet/physicallayer/base/packetlevel/MqamModulationBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandNoiseBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandRadioBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandReceiverBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandReceptionBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandTransmissionBase.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandTransmitterBase.o \
    $O/inet/physicallayer/base/packetlevel/NoiseBase.o \
    $O/inet/physicallayer/base/packetlevel/PathLossBase.o \
    $O/inet/physicallayer/base/packetlevel/PhysicalLayerBase.o \
    $O/inet/physicallayer/base/packetlevel/PpmModulationBase.o \
    $O/inet/physicallayer/base/packetlevel/PropagationBase.o \
    $O/inet/physicallayer/base/packetlevel/ReceiverBase.o \
    $O/inet/physicallayer/base/packetlevel/ReceptionBase.o \
    $O/inet/physicallayer/base/packetlevel/ScalarAnalogModelBase.o \
    $O/inet/physicallayer/base/packetlevel/SnirBase.o \
    $O/inet/physicallayer/base/packetlevel/SnirReceiverBase.o \
    $O/inet/physicallayer/base/packetlevel/TracingObstacleLossBase.o \
    $O/inet/physicallayer/base/packetlevel/TransmissionBase.o \
    $O/inet/physicallayer/base/packetlevel/TransmitterBase.o \
    $O/inet/physicallayer/common/bitlevel/AdditiveScrambler.o \
    $O/inet/physicallayer/common/bitlevel/AdditiveScramblerModule.o \
    $O/inet/physicallayer/common/bitlevel/AdditiveScrambling.o \
    $O/inet/physicallayer/common/bitlevel/AnalogDigitalConverter.o \
    $O/inet/physicallayer/common/bitlevel/ConvolutionalCode.o \
    $O/inet/physicallayer/common/bitlevel/ConvolutionalCoder.o \
    $O/inet/physicallayer/common/bitlevel/ConvolutionalCoderModule.o \
    $O/inet/physicallayer/common/bitlevel/DigitalAnalogConverter.o \
    $O/inet/physicallayer/common/bitlevel/LayeredReception.o \
    $O/inet/physicallayer/common/bitlevel/LayeredReceptionResult.o \
    $O/inet/physicallayer/common/bitlevel/LayeredTransmission.o \
    $O/inet/physicallayer/common/bitlevel/PulseFilter.o \
    $O/inet/physicallayer/common/bitlevel/PulseShaper.o \
    $O/inet/physicallayer/common/bitlevel/SignalBitModel.o \
    $O/inet/physicallayer/common/bitlevel/SignalPacketModel.o \
    $O/inet/physicallayer/common/bitlevel/SignalSampleModel.o \
    $O/inet/physicallayer/common/bitlevel/SignalSymbolModel.o \
    $O/inet/physicallayer/common/packetlevel/Arrival.o \
    $O/inet/physicallayer/common/packetlevel/BandListening.o \
    $O/inet/physicallayer/common/packetlevel/CommunicationLog.o \
    $O/inet/physicallayer/common/packetlevel/Interference.o \
    $O/inet/physicallayer/common/packetlevel/ListeningDecision.o \
    $O/inet/physicallayer/common/packetlevel/MediumLimitCache.o \
    $O/inet/physicallayer/common/packetlevel/NoiseDimensionalTransmitter.o \
    $O/inet/physicallayer/common/packetlevel/NoiseSource.o \
    $O/inet/physicallayer/common/packetlevel/Radio.o \
    $O/inet/physicallayer/common/packetlevel/RadioMedium.o \
    $O/inet/physicallayer/common/packetlevel/ReceptionDecision.o \
    $O/inet/physicallayer/common/packetlevel/ReceptionResult.o \
    $O/inet/physicallayer/common/packetlevel/Signal.o \
    $O/inet/physicallayer/communicationcache/MapCommunicationCache.o \
    $O/inet/physicallayer/communicationcache/ReferenceCommunicationCache.o \
    $O/inet/physicallayer/communicationcache/VectorCommunicationCache.o \
    $O/inet/physicallayer/contract/packetlevel/INew.o \
    $O/inet/physicallayer/contract/packetlevel/IRadio.o \
    $O/inet/physicallayer/contract/packetlevel/IRadioMedium.o \
    $O/inet/physicallayer/contract/packetlevel/IRadioSignal.o \
    $O/inet/physicallayer/contract/packetlevel/ITracingObstacleLoss.o \
    $O/inet/physicallayer/contract/packetlevel/ITransmission.o \
    $O/inet/physicallayer/energyconsumer/StateBasedCcEnergyConsumer.o \
    $O/inet/physicallayer/energyconsumer/StateBasedEpEnergyConsumer.o \
    $O/inet/physicallayer/errormodel/bitlevel/StochasticLayeredErrorModel.o \
    $O/inet/physicallayer/errormodel/packetlevel/StochasticErrorModel.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211ConvolutionalCode.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211HtInterleaving.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211LayeredOfdmReceiver.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211LayeredOfdmTransmitter.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211LayeredTransmission.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmDecoder.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmDecoderModule.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmDemodulator.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmDemodulatorModule.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmEncoder.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmEncoderModule.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmInterleaver.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmInterleaverModule.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmInterleaving.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmModulator.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmModulatorModule.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmRadio.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmSymbol.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmSymbolModel.o \
    $O/inet/physicallayer/ieee80211/bitlevel/Ieee80211VhtInterleaving.o \
    $O/inet/physicallayer/ieee80211/bitlevel/errormodel/Ieee80211OfdmErrorModel.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211Band.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211Channel.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211DsssMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211DsssOfdmMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211ErpOfdmMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211FhssMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211HrDsssMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211HtCode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211HtMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211IrMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211ModeBase.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211OfdmCode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211OfdmMode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211OfdmModulation.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211VhtCode.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211VhtMode.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211DimensionalReceiver.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211DimensionalTransmission.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211DimensionalTransmitter.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211PhyHeaderSerializer.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211PhyProtocolDissector.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211PhyProtocolPrinter.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211Radio.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ReceiverBase.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarReceiver.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarTransmission.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarTransmitter.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmissionBase.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmitterBase.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211UnitDiskReceiver.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211UnitDiskTransmission.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211UnitDiskTransmitter.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/BerParseFile.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/DsssErrorRateModel.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/Ieee80211BerTableErrorModel.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/Ieee80211ErrorModelBase.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/Ieee80211NistErrorModel.o \
    $O/inet/physicallayer/ieee80211/packetlevel/errormodel/Ieee80211YansErrorModel.o \
    $O/inet/physicallayer/ieee802154/bitlevel/Ieee802154UwbIrMode.o \
    $O/inet/physicallayer/ieee802154/bitlevel/Ieee802154UwbIrReceiver.o \
    $O/inet/physicallayer/ieee802154/bitlevel/Ieee802154UwbIrTransmitter.o \
    $O/inet/physicallayer/ieee802154/packetlevel/Ieee802154NarrowbandDimensionalReceiver.o \
    $O/inet/physicallayer/ieee802154/packetlevel/Ieee802154NarrowbandDimensionalTransmitter.o \
    $O/inet/physicallayer/ieee802154/packetlevel/Ieee802154NarrowbandScalarReceiver.o \
    $O/inet/physicallayer/ieee802154/packetlevel/Ieee802154NarrowbandScalarTransmitter.o \
    $O/inet/physicallayer/modulation/16PpmModulation.o \
    $O/inet/physicallayer/modulation/2GfskModulation.o \
    $O/inet/physicallayer/modulation/4GfskModulation.o \
    $O/inet/physicallayer/modulation/4PpmModulation.o \
    $O/inet/physicallayer/modulation/BpskModulation.o \
    $O/inet/physicallayer/modulation/DbpskModulation.o \
    $O/inet/physicallayer/modulation/DqpskModulation.o \
    $O/inet/physicallayer/modulation/DsssOqpsk16Modulation.o \
    $O/inet/physicallayer/modulation/MaskModulation.o \
    $O/inet/physicallayer/modulation/MfskModulation.o \
    $O/inet/physicallayer/modulation/MpskModulation.o \
    $O/inet/physicallayer/modulation/MqamModulation.o \
    $O/inet/physicallayer/modulation/Qam1024Modulation.o \
    $O/inet/physicallayer/modulation/Qam16Modulation.o \
    $O/inet/physicallayer/modulation/Qam256Modulation.o \
    $O/inet/physicallayer/modulation/Qam64Modulation.o \
    $O/inet/physicallayer/modulation/QbpskModulation.o \
    $O/inet/physicallayer/modulation/QpskModulation.o \
    $O/inet/physicallayer/neighborcache/GridNeighborCache.o \
    $O/inet/physicallayer/neighborcache/NeighborListNeighborCache.o \
    $O/inet/physicallayer/neighborcache/QuadTreeNeighborCache.o \
    $O/inet/physicallayer/obstacleloss/DielectricObstacleLoss.o \
    $O/inet/physicallayer/obstacleloss/IdealObstacleLoss.o \
    $O/inet/physicallayer/pathloss/BreakpointPathLoss.o \
    $O/inet/physicallayer/pathloss/FreeSpacePathLoss.o \
    $O/inet/physicallayer/pathloss/LogNormalShadowing.o \
    $O/inet/physicallayer/pathloss/NakagamiFading.o \
    $O/inet/physicallayer/pathloss/RayleighFading.o \
    $O/inet/physicallayer/pathloss/RicianFading.o \
    $O/inet/physicallayer/pathloss/SuiPathLoss.o \
    $O/inet/physicallayer/pathloss/TwoRayGroundReflection.o \
    $O/inet/physicallayer/pathloss/TwoRayInterference.o \
    $O/inet/physicallayer/pathloss/UwbIrStochasticPathLoss.o \
    $O/inet/physicallayer/propagation/ConstantSpeedPropagation.o \
    $O/inet/physicallayer/propagation/ConstantTimePropagation.o \
    $O/inet/physicallayer/shortcut/ShortcutPhyProtocolDissector.o \
    $O/inet/physicallayer/shortcut/ShortcutRadio.o \
    $O/inet/physicallayer/unitdisk/UnitDiskAnalogModel.o \
    $O/inet/physicallayer/unitdisk/UnitDiskListening.o \
    $O/inet/physicallayer/unitdisk/UnitDiskNoise.o \
    $O/inet/physicallayer/unitdisk/UnitDiskPhyHeaderSerializer.o \
    $O/inet/physicallayer/unitdisk/UnitDiskProtocolDissector.o \
    $O/inet/physicallayer/unitdisk/UnitDiskRadio.o \
    $O/inet/physicallayer/unitdisk/UnitDiskReceiver.o \
    $O/inet/physicallayer/unitdisk/UnitDiskReception.o \
    $O/inet/physicallayer/unitdisk/UnitDiskSnir.o \
    $O/inet/physicallayer/unitdisk/UnitDiskTransmission.o \
    $O/inet/physicallayer/unitdisk/UnitDiskTransmitter.o \
    $O/inet/power/base/CcEnergySinkBase.o \
    $O/inet/power/base/CcEnergySourceBase.o \
    $O/inet/power/base/CcEnergyStorageBase.o \
    $O/inet/power/base/EnergySinkBase.o \
    $O/inet/power/base/EnergySourceBase.o \
    $O/inet/power/base/EnergyStorageBase.o \
    $O/inet/power/base/EpEnergySinkBase.o \
    $O/inet/power/base/EpEnergySourceBase.o \
    $O/inet/power/base/EpEnergyStorageBase.o \
    $O/inet/power/consumer/AlternatingEpEnergyConsumer.o \
    $O/inet/power/contract/ICcEnergyConsumer.o \
    $O/inet/power/contract/ICcEnergyGenerator.o \
    $O/inet/power/contract/ICcEnergySink.o \
    $O/inet/power/contract/ICcEnergySource.o \
    $O/inet/power/contract/ICcEnergyStorage.o \
    $O/inet/power/contract/IEpEnergyConsumer.o \
    $O/inet/power/contract/IEpEnergyGenerator.o \
    $O/inet/power/contract/IEpEnergySink.o \
    $O/inet/power/contract/IEpEnergySource.o \
    $O/inet/power/contract/IEpEnergyStorage.o \
    $O/inet/power/generator/AlternatingEpEnergyGenerator.o \
    $O/inet/power/management/SimpleEpEnergyManagement.o \
    $O/inet/power/storage/IdealEpEnergyStorage.o \
    $O/inet/power/storage/SimpleCcBattery.o \
    $O/inet/power/storage/SimpleEpEnergyStorage.o \
    $O/inet/queueing/base/PacketBufferBase.o \
    $O/inet/queueing/base/PacketClassifierBase.o \
    $O/inet/queueing/base/PacketDuplicatorBase.o \
    $O/inet/queueing/base/PacketFilterBase.o \
    $O/inet/queueing/base/PacketLabelerBase.o \
    $O/inet/queueing/base/PacketMarkerBase.o \
    $O/inet/queueing/base/PacketMeterBase.o \
    $O/inet/queueing/base/PacketProcessorBase.o \
    $O/inet/queueing/base/PacketQueueBase.o \
    $O/inet/queueing/base/PacketQueueingElementBase.o \
    $O/inet/queueing/base/PacketSchedulerBase.o \
    $O/inet/queueing/base/PacketServerBase.o \
    $O/inet/queueing/base/PacketSinkBase.o \
    $O/inet/queueing/base/PacketSourceBase.o \
    $O/inet/queueing/base/PacketTaggerBase.o \
    $O/inet/queueing/base/PassivePacketSinkBase.o \
    $O/inet/queueing/base/TokenGeneratorBase.o \
    $O/inet/queueing/buffer/PacketBuffer.o \
    $O/inet/queueing/classifier/ContentBasedClassifier.o \
    $O/inet/queueing/classifier/LabelClassifier.o \
    $O/inet/queueing/classifier/MarkovClassifier.o \
    $O/inet/queueing/classifier/PacketClassifier.o \
    $O/inet/queueing/classifier/PriorityClassifier.o \
    $O/inet/queueing/classifier/WrrClassifier.o \
    $O/inet/queueing/common/OrdinalBasedDuplicator.o \
    $O/inet/queueing/common/PacketCloner.o \
    $O/inet/queueing/common/PacketDelayer.o \
    $O/inet/queueing/common/PacketDemultiplexer.o \
    $O/inet/queueing/common/PacketDuplicator.o \
    $O/inet/queueing/common/PacketGate.o \
    $O/inet/queueing/common/PacketMultiplexer.o \
    $O/inet/queueing/compat/cpacketqueue.o \
    $O/inet/queueing/compat/cqueue.o \
    $O/inet/queueing/filter/ContentBasedFilter.o \
    $O/inet/queueing/filter/OrdinalBasedDropper.o \
    $O/inet/queueing/filter/PacketFilter.o \
    $O/inet/queueing/filter/PccpIndicator.o \
    $O/inet/queueing/filter/RateLimiter.o \
    $O/inet/queueing/filter/RedDropper.o \
    $O/inet/queueing/function/PacketClassifierFunction.o \
    $O/inet/queueing/function/PacketComparatorFunction.o \
    $O/inet/queueing/function/PacketDropperFunction.o \
    $O/inet/queueing/function/PacketFilterFunction.o \
    $O/inet/queueing/function/PacketSchedulerFunction.o \
    $O/inet/queueing/marker/ContentBasedLabeler.o \
    $O/inet/queueing/marker/ContentBasedTagger.o \
    $O/inet/queueing/marker/EcnMarker.o \
    $O/inet/queueing/marker/PacketLabeler.o \
    $O/inet/queueing/marker/PacketTagger.o \
    $O/inet/queueing/meter/RateMeter.o \
    $O/inet/queueing/queue/CompoundPacketQueue.o \
    $O/inet/queueing/queue/PacketQueue.o \
    $O/inet/queueing/scheduler/ContentBasedScheduler.o \
    $O/inet/queueing/scheduler/LabelScheduler.o \
    $O/inet/queueing/scheduler/MarkovScheduler.o \
    $O/inet/queueing/scheduler/PacketScheduler.o \
    $O/inet/queueing/scheduler/PriorityScheduler.o \
    $O/inet/queueing/scheduler/WrrScheduler.o \
    $O/inet/queueing/server/PacketServer.o \
    $O/inet/queueing/server/TokenBasedServer.o \
    $O/inet/queueing/sink/ActivePacketSink.o \
    $O/inet/queueing/sink/PassivePacketSink.o \
    $O/inet/queueing/sink/PcapFilePacketConsumer.o \
    $O/inet/queueing/source/ActivePacketSource.o \
    $O/inet/queueing/source/EmptyPacketSource.o \
    $O/inet/queueing/source/PassivePacketSource.o \
    $O/inet/queueing/source/PcapFilePacketProducer.o \
    $O/inet/queueing/tokengenerator/PacketBasedTokenGenerator.o \
    $O/inet/queueing/tokengenerator/QueueBasedTokenGenerator.o \
    $O/inet/queueing/tokengenerator/SignalBasedTokenGenerator.o \
    $O/inet/queueing/tokengenerator/TimeBasedTokenGenerator.o \
    $O/inet/routing/aodv_cluster/Aodv_cluster.o \
    $O/inet/routing/aodv_cluster/AodvRouteData.o \
    $O/inet/routing/common/ManetProtocolDissector.o \
    $O/inet/routing/dsdv/Dsdv.o \
    $O/inet/routing/ospf_common/OspfPacketSerializer.o \
    $O/inet/routing/ospf_common/OspfProtocolDissector.o \
    $O/inet/transportlayer/base/TransportProtocolBase.o \
    $O/inet/transportlayer/common/CrcMode.o \
    $O/inet/transportlayer/common/L4Tools.o \
    $O/inet/transportlayer/common/TransportPseudoHeaderSerializer.o \
    $O/inet/transportlayer/contract/sctp/SctpSocket.o \
    $O/inet/transportlayer/contract/tcp/TcpSocket.o \
    $O/inet/transportlayer/contract/udp/UdpSocket.o \
    $O/inet/transportlayer/udp/Udp.o \
    $O/inet/transportlayer/udp/UdpHeader.o \
    $O/inet/transportlayer/udp/UdpHeaderSerializer.o \
    $O/inet/transportlayer/udp/UdpProtocolDissector.o \
    $O/inet/transportlayer/udp/UdpProtocolPrinter.o \
    $O/inet/visualizer/base/EnergyStorageVisualizerBase.o \
    $O/inet/visualizer/base/Ieee80211VisualizerBase.o \
    $O/inet/visualizer/base/InfoVisualizerBase.o \
    $O/inet/visualizer/base/InterfaceTableVisualizerBase.o \
    $O/inet/visualizer/base/LinkBreakVisualizerBase.o \
    $O/inet/visualizer/base/LinkCanvasVisualizerBase.o \
    $O/inet/visualizer/base/LinkOsgVisualizerBase.o \
    $O/inet/visualizer/base/LinkVisualizerBase.o \
    $O/inet/visualizer/base/MediumVisualizerBase.o \
    $O/inet/visualizer/base/MobilityVisualizerBase.o \
    $O/inet/visualizer/base/NetworkConnectionVisualizerBase.o \
    $O/inet/visualizer/base/NetworkNodeVisualizerBase.o \
    $O/inet/visualizer/base/PacketDropVisualizerBase.o \
    $O/inet/visualizer/base/PathCanvasVisualizerBase.o \
    $O/inet/visualizer/base/PathOsgVisualizerBase.o \
    $O/inet/visualizer/base/PathVisualizerBase.o \
    $O/inet/visualizer/base/PhysicalEnvironmentVisualizerBase.o \
    $O/inet/visualizer/base/QueueVisualizerBase.o \
    $O/inet/visualizer/base/RadioVisualizerBase.o \
    $O/inet/visualizer/base/RoutingTableVisualizerBase.o \
    $O/inet/visualizer/base/SceneOsgVisualizerBase.o \
    $O/inet/visualizer/base/SceneVisualizerBase.o \
    $O/inet/visualizer/base/StatisticVisualizerBase.o \
    $O/inet/visualizer/base/TracingObstacleLossVisualizerBase.o \
    $O/inet/visualizer/base/TransportConnectionVisualizerBase.o \
    $O/inet/visualizer/base/VisualizerBase.o \
    $O/inet/visualizer/common/InfoCanvasVisualizer.o \
    $O/inet/visualizer/common/InfoOsgVisualizer.o \
    $O/inet/visualizer/common/PacketDropCanvasVisualizer.o \
    $O/inet/visualizer/common/PacketDropOsgVisualizer.o \
    $O/inet/visualizer/common/QueueCanvasVisualizer.o \
    $O/inet/visualizer/common/QueueOsgVisualizer.o \
    $O/inet/visualizer/common/StatisticCanvasVisualizer.o \
    $O/inet/visualizer/common/StatisticOsgVisualizer.o \
    $O/inet/visualizer/environment/PhysicalEnvironmentCanvasVisualizer.o \
    $O/inet/visualizer/environment/PhysicalEnvironmentOsgVisualizer.o \
    $O/inet/visualizer/linklayer/DataLinkCanvasVisualizer.o \
    $O/inet/visualizer/linklayer/DataLinkOsgVisualizer.o \
    $O/inet/visualizer/linklayer/Ieee80211CanvasVisualizer.o \
    $O/inet/visualizer/linklayer/Ieee80211OsgVisualizer.o \
    $O/inet/visualizer/linklayer/InterfaceTableCanvasVisualizer.o \
    $O/inet/visualizer/linklayer/InterfaceTableOsgVisualizer.o \
    $O/inet/visualizer/linklayer/LinkBreakCanvasVisualizer.o \
    $O/inet/visualizer/linklayer/LinkBreakOsgVisualizer.o \
    $O/inet/visualizer/mobility/MobilityCanvasVisualizer.o \
    $O/inet/visualizer/mobility/MobilityOsgVisualizer.o \
    $O/inet/visualizer/networklayer/NetworkRouteCanvasVisualizer.o \
    $O/inet/visualizer/networklayer/NetworkRouteOsgVisualizer.o \
    $O/inet/visualizer/networklayer/RoutingTableCanvasVisualizer.o \
    $O/inet/visualizer/networklayer/RoutingTableOsgVisualizer.o \
    $O/inet/visualizer/physicallayer/MediumCanvasVisualizer.o \
    $O/inet/visualizer/physicallayer/MediumOsgVisualizer.o \
    $O/inet/visualizer/physicallayer/PhysicalLinkCanvasVisualizer.o \
    $O/inet/visualizer/physicallayer/PhysicalLinkOsgVisualizer.o \
    $O/inet/visualizer/physicallayer/RadioCanvasVisualizer.o \
    $O/inet/visualizer/physicallayer/RadioOsgVisualizer.o \
    $O/inet/visualizer/physicallayer/TracingObstacleLossCanvasVisualizer.o \
    $O/inet/visualizer/physicallayer/TracingObstacleLossOsgVisualizer.o \
    $O/inet/visualizer/power/EnergyStorageCanvasVisualizer.o \
    $O/inet/visualizer/power/EnergyStorageOsgVisualizer.o \
    $O/inet/visualizer/scene/NetworkConnectionCanvasVisualizer.o \
    $O/inet/visualizer/scene/NetworkConnectionOsgVisualizer.o \
    $O/inet/visualizer/scene/NetworkNodeCanvasVisualization.o \
    $O/inet/visualizer/scene/NetworkNodeCanvasVisualizer.o \
    $O/inet/visualizer/scene/NetworkNodeOsgVisualization.o \
    $O/inet/visualizer/scene/NetworkNodeOsgVisualizer.o \
    $O/inet/visualizer/scene/OpenStreetMapSceneCanvasVisualizer.o \
    $O/inet/visualizer/scene/SceneCanvasVisualizer.o \
    $O/inet/visualizer/scene/SceneOsgEarthVisualizer.o \
    $O/inet/visualizer/scene/SceneOsgVisualizer.o \
    $O/inet/visualizer/transportlayer/TransportConnectionCanvasVisualizer.o \
    $O/inet/visualizer/transportlayer/TransportConnectionOsgVisualizer.o \
    $O/inet/visualizer/transportlayer/TransportRouteCanvasVisualizer.o \
    $O/inet/visualizer/transportlayer/TransportRouteOsgVisualizer.o \
    $O/inet/visualizer/util/AnimationPosition.o \
    $O/inet/visualizer/util/AnimationSpeedInterpolator.o \
    $O/inet/visualizer/util/ColorSet.o \
    $O/inet/visualizer/util/InterfaceFilter.o \
    $O/inet/visualizer/util/LineManager.o \
    $O/inet/visualizer/util/ModuleFilter.o \
    $O/inet/visualizer/util/NetworkNodeFilter.o \
    $O/inet/visualizer/util/Placement.o \
    $O/inet/visualizer/util/PortFilter.o \
    $O/inet/visualizer/util/QueueFilter.o \
    $O/inet/applications/Fap/FAPmessage_m.o \
    $O/inet/applications/base/ApplicationPacket_m.o \
    $O/inet/applications/common/SocketTag_m.o \
    $O/inet/applications/netperfmeter/NetPerfMeter_m.o \
    $O/inet/applications/pingapp/PingApp_m.o \
    $O/inet/common/INETDefs_m.o \
    $O/inet/common/Protocol_m.o \
    $O/inet/common/ProtocolTag_m.o \
    $O/inet/common/Simsignals_m.o \
    $O/inet/common/TagBase_m.o \
    $O/inet/common/TimeTag_m.o \
    $O/inet/common/TlvOptions_m.o \
    $O/inet/common/Units_m.o \
    $O/inet/common/geometry/Geometry_m.o \
    $O/inet/common/packet/ChunkBuffer_m.o \
    $O/inet/common/packet/ChunkQueue_m.o \
    $O/inet/common/packet/Message_m.o \
    $O/inet/common/packet/Packet_m.o \
    $O/inet/common/packet/chunk/Chunk_m.o \
    $O/inet/common/scenario/ScenarioTimer_m.o \
    $O/inet/environment/common/Material_m.o \
    $O/inet/environment/common/PhysicalObject_m.o \
    $O/inet/linklayer/acking/AckingMacHeader_m.o \
    $O/inet/linklayer/common/EtherType_m.o \
    $O/inet/linklayer/common/FcsMode_m.o \
    $O/inet/linklayer/common/Ieee802Ctrl_m.o \
    $O/inet/linklayer/common/Ieee802SapTag_m.o \
    $O/inet/linklayer/common/InterfaceTag_m.o \
    $O/inet/linklayer/common/MacAddress_m.o \
    $O/inet/linklayer/common/MacAddressTag_m.o \
    $O/inet/linklayer/common/UserPriorityTag_m.o \
    $O/inet/linklayer/common/VlanTag_m.o \
    $O/inet/linklayer/csmaca/CsmaCaMacHeader_m.o \
    $O/inet/linklayer/ieee80211/llc/Ieee80211EtherTypeHeader_m.o \
    $O/inet/linklayer/ieee80211/llc/LlcProtocolTag_m.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211Frame_m.o \
    $O/inet/linklayer/ieee80211/mac/Ieee80211SubtypeTag_m.o \
    $O/inet/linklayer/ieee80211/mac/framesequence/FrameSequence_m.o \
    $O/inet/linklayer/ieee80211/mac/queue/InProgressFrames_m.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211MgmtFrame_m.o \
    $O/inet/linklayer/ieee80211/mgmt/Ieee80211Primitives_m.o \
    $O/inet/linklayer/ieee802154/Ieee802154MacHeader_m.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcHeader_m.o \
    $O/inet/linklayer/ieee8022/Ieee8022LlcSocketCommand_m.o \
    $O/inet/linklayer/shortcut/ShortcutMacHeader_m.o \
    $O/inet/networklayer/arp/ipv4/ArpPacket_m.o \
    $O/inet/networklayer/common/ClTag_m.o \
    $O/inet/networklayer/common/DscpTag_m.o \
    $O/inet/networklayer/common/EchoPacket_m.o \
    $O/inet/networklayer/common/EcnTag_m.o \
    $O/inet/networklayer/common/FragmentationTag_m.o \
    $O/inet/networklayer/common/HopLimitTag_m.o \
    $O/inet/networklayer/common/InterfaceEntry_m.o \
    $O/inet/networklayer/common/IpProtocolId_m.o \
    $O/inet/networklayer/common/L3Address_m.o \
    $O/inet/networklayer/common/L3AddressTag_m.o \
    $O/inet/networklayer/common/MulticastTag_m.o \
    $O/inet/networklayer/common/NextHopAddressTag_m.o \
    $O/inet/networklayer/common/PitTag_m.o \
    $O/inet/networklayer/common/SidTag_m.o \
    $O/inet/networklayer/common/TosTag_m.o \
    $O/inet/networklayer/contract/L3SocketCommand_m.o \
    $O/inet/networklayer/contract/NetworkHeaderBase_m.o \
    $O/inet/networklayer/contract/clns/ClnsAddress_m.o \
    $O/inet/networklayer/contract/color/ColorSocketCommand_m.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Address_m.o \
    $O/inet/networklayer/contract/ipv4/Ipv4SocketCommand_m.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Address_m.o \
    $O/inet/networklayer/contract/ipv6/Ipv6SocketCommand_m.o \
    $O/inet/networklayer/flooding/FloodingHeader_m.o \
    $O/inet/networklayer/icn/cluster/SimpleClusterPacket_m.o \
    $O/inet/networklayer/icn/color/AppData_m.o \
    $O/inet/networklayer/icn/color/colorfields_m.o \
    $O/inet/networklayer/icn/color/Data_m.o \
    $O/inet/networklayer/icn/color/Get_m.o \
    $O/inet/networklayer/icn/pendingTable/PITentry_m.o \
    $O/inet/networklayer/icn/routingTable/Croute_m.o \
    $O/inet/networklayer/ipv4/IcmpHeader_m.o \
    $O/inet/networklayer/ipv4/IgmpMessage_m.o \
    $O/inet/networklayer/ipv4/Ipv4Header_m.o \
    $O/inet/networklayer/ipv4/Ipv4NatEntry_m.o \
    $O/inet/networklayer/ipv4/Ipv4OptionsTag_m.o \
    $O/inet/networklayer/ipv4/Ipv4Route_m.o \
    $O/inet/networklayer/nexthop/NextHopForwardingHeader_m.o \
    $O/inet/networklayer/probabilistic/ProbabilisticBroadcastHeader_m.o \
    $O/inet/networklayer/wiseroute/WiseRouteHeader_m.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarReception_m.o \
    $O/inet/physicallayer/analogmodel/packetlevel/ScalarTransmission_m.o \
    $O/inet/physicallayer/apskradio/packetlevel/ApskPhyHeader_m.o \
    $O/inet/physicallayer/base/packetlevel/FlatReceptionBase_m.o \
    $O/inet/physicallayer/base/packetlevel/FlatTransmissionBase_m.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandReceptionBase_m.o \
    $O/inet/physicallayer/base/packetlevel/NarrowbandTransmissionBase_m.o \
    $O/inet/physicallayer/base/packetlevel/ReceptionBase_m.o \
    $O/inet/physicallayer/base/packetlevel/TransmissionBase_m.o \
    $O/inet/physicallayer/common/packetlevel/Arrival_m.o \
    $O/inet/physicallayer/common/packetlevel/BandListening_m.o \
    $O/inet/physicallayer/common/packetlevel/Signal_m.o \
    $O/inet/physicallayer/contract/packetlevel/IModulation_m.o \
    $O/inet/physicallayer/contract/packetlevel/RadioControlInfo_m.o \
    $O/inet/physicallayer/contract/packetlevel/SignalTag_m.o \
    $O/inet/physicallayer/ieee80211/mode/Ieee80211Channel_m.o \
    $O/inet/physicallayer/ieee80211/mode/IIeee80211Mode_m.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211DimensionalTransmission_m.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211PhyHeader_m.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarTransmission_m.o \
    $O/inet/physicallayer/ieee80211/packetlevel/Ieee80211Tag_m.o \
    $O/inet/physicallayer/shortcut/ShortcutPhyHeader_m.o \
    $O/inet/physicallayer/unitdisk/UnitDiskPhyHeader_m.o \
    $O/inet/physicallayer/unitdisk/UnitDiskTransmission_m.o \
    $O/inet/queueing/common/LabelsTag_m.o \
    $O/inet/queueing/common/RateTag_m.o \
    $O/inet/routing/aodv_cluster/AodvControlPackets_m.o \
    $O/inet/routing/dsdv/DsdvHello_m.o \
    $O/inet/routing/ospf_common/OspfPacketBase_m.o \
    $O/inet/transportlayer/common/CrcMode_m.o \
    $O/inet/transportlayer/common/L4PortTag_m.o \
    $O/inet/transportlayer/common/TransportPseudoHeader_m.o \
    $O/inet/transportlayer/contract/TransportHeaderBase_m.o \
    $O/inet/transportlayer/contract/sctp/SctpCommand_m.o \
    $O/inet/transportlayer/contract/tcp/TcpCommand_m.o \
    $O/inet/transportlayer/contract/udp/UdpControlInfo_m.o \
    $O/inet/transportlayer/udp/UdpHeader_m.o \
    $O/inet/visualizer/base/PacketDrop_m.o

# Message files
MSGFILES = \
    inet/applications/Fap/FAPmessage.msg \
    inet/applications/base/ApplicationPacket.msg \
    inet/applications/common/SocketTag.msg \
    inet/applications/netperfmeter/NetPerfMeter.msg \
    inet/applications/pingapp/PingApp.msg \
    inet/common/INETDefs.msg \
    inet/common/Protocol.msg \
    inet/common/ProtocolTag.msg \
    inet/common/Simsignals.msg \
    inet/common/TagBase.msg \
    inet/common/TimeTag.msg \
    inet/common/TlvOptions.msg \
    inet/common/Units.msg \
    inet/common/geometry/Geometry.msg \
    inet/common/packet/ChunkBuffer.msg \
    inet/common/packet/ChunkQueue.msg \
    inet/common/packet/Message.msg \
    inet/common/packet/Packet.msg \
    inet/common/packet/chunk/Chunk.msg \
    inet/common/scenario/ScenarioTimer.msg \
    inet/environment/common/Material.msg \
    inet/environment/common/PhysicalObject.msg \
    inet/linklayer/acking/AckingMacHeader.msg \
    inet/linklayer/common/EtherType.msg \
    inet/linklayer/common/FcsMode.msg \
    inet/linklayer/common/Ieee802Ctrl.msg \
    inet/linklayer/common/Ieee802SapTag.msg \
    inet/linklayer/common/InterfaceTag.msg \
    inet/linklayer/common/MacAddress.msg \
    inet/linklayer/common/MacAddressTag.msg \
    inet/linklayer/common/UserPriorityTag.msg \
    inet/linklayer/common/VlanTag.msg \
    inet/linklayer/csmaca/CsmaCaMacHeader.msg \
    inet/linklayer/ieee80211/llc/Ieee80211EtherTypeHeader.msg \
    inet/linklayer/ieee80211/llc/LlcProtocolTag.msg \
    inet/linklayer/ieee80211/mac/Ieee80211Frame.msg \
    inet/linklayer/ieee80211/mac/Ieee80211SubtypeTag.msg \
    inet/linklayer/ieee80211/mac/framesequence/FrameSequence.msg \
    inet/linklayer/ieee80211/mac/queue/InProgressFrames.msg \
    inet/linklayer/ieee80211/mgmt/Ieee80211MgmtFrame.msg \
    inet/linklayer/ieee80211/mgmt/Ieee80211Primitives.msg \
    inet/linklayer/ieee802154/Ieee802154MacHeader.msg \
    inet/linklayer/ieee8022/Ieee8022LlcHeader.msg \
    inet/linklayer/ieee8022/Ieee8022LlcSocketCommand.msg \
    inet/linklayer/shortcut/ShortcutMacHeader.msg \
    inet/networklayer/arp/ipv4/ArpPacket.msg \
    inet/networklayer/common/ClTag.msg \
    inet/networklayer/common/DscpTag.msg \
    inet/networklayer/common/EchoPacket.msg \
    inet/networklayer/common/EcnTag.msg \
    inet/networklayer/common/FragmentationTag.msg \
    inet/networklayer/common/HopLimitTag.msg \
    inet/networklayer/common/InterfaceEntry.msg \
    inet/networklayer/common/IpProtocolId.msg \
    inet/networklayer/common/L3Address.msg \
    inet/networklayer/common/L3AddressTag.msg \
    inet/networklayer/common/MulticastTag.msg \
    inet/networklayer/common/NextHopAddressTag.msg \
    inet/networklayer/common/PitTag.msg \
    inet/networklayer/common/SidTag.msg \
    inet/networklayer/common/TosTag.msg \
    inet/networklayer/contract/L3SocketCommand.msg \
    inet/networklayer/contract/NetworkHeaderBase.msg \
    inet/networklayer/contract/clns/ClnsAddress.msg \
    inet/networklayer/contract/color/ColorSocketCommand.msg \
    inet/networklayer/contract/ipv4/Ipv4Address.msg \
    inet/networklayer/contract/ipv4/Ipv4SocketCommand.msg \
    inet/networklayer/contract/ipv6/Ipv6Address.msg \
    inet/networklayer/contract/ipv6/Ipv6SocketCommand.msg \
    inet/networklayer/flooding/FloodingHeader.msg \
    inet/networklayer/icn/cluster/SimpleClusterPacket.msg \
    inet/networklayer/icn/color/AppData.msg \
    inet/networklayer/icn/color/colorfields.msg \
    inet/networklayer/icn/color/Data.msg \
    inet/networklayer/icn/color/Get.msg \
    inet/networklayer/icn/pendingTable/PITentry.msg \
    inet/networklayer/icn/routingTable/Croute.msg \
    inet/networklayer/ipv4/IcmpHeader.msg \
    inet/networklayer/ipv4/IgmpMessage.msg \
    inet/networklayer/ipv4/Ipv4Header.msg \
    inet/networklayer/ipv4/Ipv4NatEntry.msg \
    inet/networklayer/ipv4/Ipv4OptionsTag.msg \
    inet/networklayer/ipv4/Ipv4Route.msg \
    inet/networklayer/nexthop/NextHopForwardingHeader.msg \
    inet/networklayer/probabilistic/ProbabilisticBroadcastHeader.msg \
    inet/networklayer/wiseroute/WiseRouteHeader.msg \
    inet/physicallayer/analogmodel/packetlevel/ScalarReception.msg \
    inet/physicallayer/analogmodel/packetlevel/ScalarTransmission.msg \
    inet/physicallayer/apskradio/packetlevel/ApskPhyHeader.msg \
    inet/physicallayer/base/packetlevel/FlatReceptionBase.msg \
    inet/physicallayer/base/packetlevel/FlatTransmissionBase.msg \
    inet/physicallayer/base/packetlevel/NarrowbandReceptionBase.msg \
    inet/physicallayer/base/packetlevel/NarrowbandTransmissionBase.msg \
    inet/physicallayer/base/packetlevel/ReceptionBase.msg \
    inet/physicallayer/base/packetlevel/TransmissionBase.msg \
    inet/physicallayer/common/packetlevel/Arrival.msg \
    inet/physicallayer/common/packetlevel/BandListening.msg \
    inet/physicallayer/common/packetlevel/Signal.msg \
    inet/physicallayer/contract/packetlevel/IModulation.msg \
    inet/physicallayer/contract/packetlevel/RadioControlInfo.msg \
    inet/physicallayer/contract/packetlevel/SignalTag.msg \
    inet/physicallayer/ieee80211/mode/Ieee80211Channel.msg \
    inet/physicallayer/ieee80211/mode/IIeee80211Mode.msg \
    inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo.msg \
    inet/physicallayer/ieee80211/packetlevel/Ieee80211DimensionalTransmission.msg \
    inet/physicallayer/ieee80211/packetlevel/Ieee80211PhyHeader.msg \
    inet/physicallayer/ieee80211/packetlevel/Ieee80211ScalarTransmission.msg \
    inet/physicallayer/ieee80211/packetlevel/Ieee80211Tag.msg \
    inet/physicallayer/shortcut/ShortcutPhyHeader.msg \
    inet/physicallayer/unitdisk/UnitDiskPhyHeader.msg \
    inet/physicallayer/unitdisk/UnitDiskTransmission.msg \
    inet/queueing/common/LabelsTag.msg \
    inet/queueing/common/RateTag.msg \
    inet/routing/aodv_cluster/AodvControlPackets.msg \
    inet/routing/dsdv/DsdvHello.msg \
    inet/routing/ospf_common/OspfPacketBase.msg \
    inet/transportlayer/common/CrcMode.msg \
    inet/transportlayer/common/L4PortTag.msg \
    inet/transportlayer/common/TransportPseudoHeader.msg \
    inet/transportlayer/contract/TransportHeaderBase.msg \
    inet/transportlayer/contract/sctp/SctpCommand.msg \
    inet/transportlayer/contract/tcp/TcpCommand.msg \
    inet/transportlayer/contract/udp/UdpControlInfo.msg \
    inet/transportlayer/udp/UdpHeader.msg \
    inet/visualizer/base/PacketDrop.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_EXPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH) -PINET_API
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
.DEFAULT_GOAL := all-pch

#
# Use the new message compiler introduced in OMNeT++ 5.3
#
MSGC:=$(MSGC) --msg6

FEATURETOOL := opp_featuretool

#
# compiler warning for c-style cast
#
#CFLAGS += -Wold-style-cast

ifeq ($(PLATFORM),win32.x86_64)
  #
  # on windows we have to link with the ws2_32 (winsock2) library as it is no longer added
  # to the omnetpp system libraries by default (as of OMNeT++ 5.1)
  #
  LIBS += -lws2_32
  DEFINES += -DINET_EXPORT
  ENABLE_AUTO_IMPORT=-Wl,--enable-auto-import
  LDFLAGS := $(filter-out $(ENABLE_AUTO_IMPORT), $(LDFLAGS))

  # FIX for clang not exporting typinfo and other thunks on Windows
  ifeq ($(CC),clang)
    DEFFILE = $O/typeinfo.def
    LDFLAGS += $(DEFFILE)

$(DEFFILE): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@echo Creating .DEF file for $(TARGET) - needed to fix an issue where clang is not exporting typinfo for classes on Windows
	$(Q)echo >.tmp$$$$ $(OBJS) $(EXTRA_OBJS) && $(DLLTOOL) --export-all-symbols --kill-at -z all$$$$.def @.tmp$$$$ && grep -E '( *_ZTI)|( *_ZThn)|EXPORTS' all$$$$.def | sed 's/ @.*//' >$(DEFFILE) && rm .tmp$$$$ all$$$$.def

$O/$(TARGET): $(DEFFILE)

  endif
endif

#
# TCP implementaion using the Network Simulation Cradle (TCP_NSC feature)
#
WITH_TCP_NSC := $(shell (cd .. && $(FEATURETOOL) -q isenabled TCP_NSC && echo enabled) )
ifeq ($(WITH_TCP_NSC), enabled)
  NSC_VERSION= $(shell ls -d ../3rdparty/nsc* 2>/dev/null | sed 's/^.*-//')
  ifneq ($(NSC_VERSION),)
    INCLUDE_PATH += -I../3rdparty/nsc-$(NSC_VERSION)/sim
    LIBS += -Wl,-rpath,$(abspath ../3rdparty/nsc-$(NSC_VERSION))
  else
    $(error Please install NSC or disable 'TCP_NSC' feature)
  endif
endif

#
# TCP implementation using the lwIP stack
#
WITH_TCP_LWIP := $(shell (cd .. && $(FEATURETOOL) -q isenabled TCP_lwIP && echo enabled) )
ifeq ($(WITH_TCP_LWIP), enabled)
  INCLUDE_PATH += -Iinet/transportlayer/tcp_lwip/lwip/include -Iinet/transportlayer/tcp_lwip/lwip/include/ipv4 -Iinet/transportlayer/tcp_lwip/lwip/include/ipv6
endif

#
# pkg-config:
#
HAVE_PKGCFG := $(shell pkg-config --version 2>/dev/null)
ifeq ($(strip $(HAVE_PKGCFG)),)
  HAVE_PKGCFG := no
else
  HAVE_PKGCFG := yes
  PKGCFG := $(shell which pkg-config)
endif

#
# VoipStream feature:
#
WITH_VOIPSTREAM := $(shell (cd .. && $(FEATURETOOL) -q isenabled VoIPStream && echo enabled) )
ifeq ($(WITH_VOIPSTREAM), enabled)
  ifeq ($(HAVE_PKGCFG), yes)
    HAVE_FFMPEG := $(shell $(PKGCFG) --exists libavcodec libavformat libavutil && echo yes || echo no)
    ifeq ($(HAVE_FFMPEG), yes)
      LIBS += $(shell $(PKGCFG) --libs libavcodec libavformat libavutil)
      CFLAGS += $(shell $(PKGCFG) --cflags libavcodec libavformat libavutil) -DHAVE_FFMPEG
    endif
    HAVE_FFMPEG_AVRESAMPLE := $(shell $(PKGCFG) --exists libavresample && echo yes || echo no)
    ifeq ($(HAVE_FFMPEG_AVRESAMPLE), yes)
      LIBS += $(shell $(PKGCFG) --libs libavresample)
      CFLAGS += $(shell $(PKGCFG) --cflags libavresample) -DHAVE_FFMPEG_AVRESAMPLE
    endif
  endif
endif

#
# visualization feature requires (optionally) some extra osg and osgEarth libraries
#
WITH_VISUALIZERS := $(shell (cd .. && $(FEATURETOOL) -q isenabled visualization && echo enabled) )
ifeq ($(WITH_VISUALIZERS), enabled)
  ifeq ($(WITH_OSG), yes)
    OMNETPP_LIBS += -losg -losgText -losgDB -losgGA -losgViewer -losgUtil -lOpenThreads # TODO: use $(OSG_LIBS) from Makefile.inc? Does that include -losgText? Why not?
    ifeq ($(WITH_OSGEARTH), yes)
      OMNETPP_LIBS += -losgEarth -losgEarthUtil # TODO: use $(OSGEARTH_LIBS) from Makefile.inc? -lgeos_c might also be needed. Is that included? Should it be?
    endif
  endif
endif

# uncomment this if you want to run the NS3 vs INET 802.11 cross validation tests in the 'tests/misc/ns3' folder.
# CFLAGS += -DNS3_VALIDATION

# disable anoying "... hides overloaded virtual function" warning
CFLAGS += -Wno-overloaded-virtual

#########################################################################
# precompiled header support for GCC and CLANG

# Set this to "no" if you want to avoid using the precompiled header. The default is "yes".
USE_PRECOMPILED_HEADER ?= yes

# force turning off precompiled headers if ccache is used as it is not compatible with precompiled headers.
ifneq (,$(findstring ccache,$(shell (which $(CC)))))
  USE_PRECOMPILED_HEADER = no
  $(info *** CCACHE detected - precompiled headers disabled)
endif

PRECOMPILED_HEADER=inet/common/precompiled_$(MODE).h

ifeq ($(USE_PRECOMPILED_HEADER),yes)
  ifneq (,$(findstring gcc,$(CC)))
    PRECOMPILED_EXT=gch
  else ifneq (,$(findstring clang,$(CC)))
    PRECOMPILED_EXT=pch
  else
    PRECOMPILED_EXT=
  endif
  CFLAGS += -include $(PRECOMPILED_HEADER)
endif

PRECOMPILED_HEADER_PCH=$(PRECOMPILED_HEADER).$(PRECOMPILED_EXT)
PRECOMPILED_HEADER_D=$(PRECOMPILED_HEADER_PCH:%.$(PRECOMPILED_EXT)=%.d)

# Main target
all-pch: | pch msgheaders all

.PHONY: pch clean-pch

pch : $(PRECOMPILED_HEADER_PCH)

$(PRECOMPILED_HEADER_PCH): $(PRECOMPILED_HEADER)
ifneq ("$(PRECOMPILED_EXT)","")
	@echo Creating precompiled header for $(CC)...
	$(Q)$(CXX) -x c++-header $(CXXFLAGS) $(filter-out -MMD -include $(PRECOMPILED_HEADER),$(COPTS)) -MD -o $@ $<
endif

clean: clean-pch clean-defines

clean-pch:
	$(Q)-rm -f $(PRECOMPILED_HEADER_D) $(PRECOMPILED_HEADER).pch $(PRECOMPILED_HEADER).gch

-include $(PRECOMPILED_HEADER_D)

# Create opp_defines.h so important WITH_* macros from OMNeT++ can be included as macros from a header file
# This helps the IDE to properly enable/disable conditional code in the editor
DEFINES_FILE=inet/opp_defines.h

msgheaders: $(DEFINES_FILE)

clean-defines:
	$(Q)-rm -f $(DEFINES_FILE)

$(DEFINES_FILE) : $(COPTS_FILE)
	@echo "// Generated file, do not edit" >$(DEFINES_FILE)
ifeq ($(WITH_OSG),yes)
	@echo "#ifndef WITH_OSG" >>$(DEFINES_FILE)
	@echo "#define WITH_OSG" >>$(DEFINES_FILE)
	@echo "#endif" >>$(DEFINES_FILE)
endif
ifeq ($(WITH_OSGEARTH),yes)
	@echo "#ifndef WITH_OSGEARTH" >>$(DEFINES_FILE)
	@echo "#define WITH_OSGEARTH" >>$(DEFINES_FILE)
	@echo "#endif" >>$(DEFINES_FILE)
endif

# dump out the actual compiler and linker command line for easier debugging
ifneq ($(MAKECMDGOALS),clean)
$(info *** COMPILING with:)
$(info $(CXX) -c $(CXXFLAGS) $(COPTS))
$(info *** LINKING with:)
$(info $(SHLIB_LD) -o $O/$(TARGET) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS))
$(info Building...)
endif

# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)