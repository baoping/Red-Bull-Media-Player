include( "../globals.pri" )
TEMPLATE=subdirs
CONFIG += ordered
SUBDIRS += \        
        Container \
        Interfaces \
        ToolLib \
        Package \
        UpdateServices \
	Events \
	EventAggregator \
        ../QTVLC \
        Widgets \
	PlayerShell \	
        Player \
        Modules \
        Test 



