#include "main.h"

/* IDs must be sequential */
struct vehicle_entry vehicle_list[VEHICLE_LIST_SIZE] =
{	// model_id, vehicle class, vehicle name, passenger seats
	{ 400, VEHICLE_CLASS_CAR, "Landstalker", 3 },
	{ 401, VEHICLE_CLASS_CAR, "Bravura", 1 },
	{ 402, VEHICLE_CLASS_CAR_FAST, "Buffalo", 1 },
	{ 403, VEHICLE_CLASS_HEAVY, "Linerunner", 1 },
	{ 404, VEHICLE_CLASS_CAR, "Perennial", 3 },
	{ 405, VEHICLE_CLASS_CAR, "Sentinel", 3 },
	{ 406, VEHICLE_CLASS_HEAVY, "Dumper", 0 },
	{ 407, VEHICLE_CLASS_HEAVY, "FireTruck", 1 },
	{ 408, VEHICLE_CLASS_HEAVY, "Trashmaster", 1 },
	{ 409, VEHICLE_CLASS_HEAVY, "Stretch", 3 },
	{ 410, VEHICLE_CLASS_CAR, "Manana", 1 },
	{ 411, VEHICLE_CLASS_CAR_FAST, "Infernus", 1 },
	{ 412, VEHICLE_CLASS_CAR, "Voodoo", 1 },
	{ 413, VEHICLE_CLASS_CAR, "Pony", 3 },
	{ 414, VEHICLE_CLASS_CAR, "Mule", 1 },
	{ 415, VEHICLE_CLASS_CAR_FAST, "Cheetah", 1 },
	{ 416, VEHICLE_CLASS_HEAVY, "Ambulance", 3 },
	{ 417, VEHICLE_CLASS_HELI, "Leviathan", 1 },
	{ 418, VEHICLE_CLASS_CAR, "Moonbeam", 3 },
	{ 419, VEHICLE_CLASS_CAR, "Esperanto", 1 },
	{ 420, VEHICLE_CLASS_CAR, "Taxi", 3 },
	{ 421, VEHICLE_CLASS_CAR, "Washington", 3 },
	{ 422, VEHICLE_CLASS_CAR, "Bobcat", 1 },
	{ 423, VEHICLE_CLASS_HEAVY, "MrWhoopee", 1 },
	{ 424, VEHICLE_CLASS_CAR, "BFInjection", 1 },
	{ 425, VEHICLE_CLASS_HELI, "Hunter", 0 },
	{ 426, VEHICLE_CLASS_CAR, "Premier", 3 },
	{ 427, VEHICLE_CLASS_HEAVY, "Enforcer", 3 },
	{ 428, VEHICLE_CLASS_HEAVY, "Securicar", 3 },
	{ 429, VEHICLE_CLASS_CAR_FAST, "Banshee", 1 },
	{ 430, VEHICLE_CLASS_BOAT, "Predator", 0 },
	{ 431, VEHICLE_CLASS_HEAVY, "Bus", 7 },
	{ 432, VEHICLE_CLASS_HEAVY, "Rhino", 0 },

	/* tank... */
	{ 433, VEHICLE_CLASS_HEAVY, "Barracks", 1 },
	{ 434, VEHICLE_CLASS_CAR_FAST, "Hotknife", 1 },
	{ 435, VEHICLE_CLASS_TRAILER, "ArtictTrailer", 0 },
	{ 436, VEHICLE_CLASS_CAR, "Previon", 1 },
	{ 437, VEHICLE_CLASS_HEAVY, "Coach", 7 },
	{ 438, VEHICLE_CLASS_CAR, "Cabbie", 3 },
	{ 439, VEHICLE_CLASS_CAR_FAST, "Stallion", 1 },
	{ 440, VEHICLE_CLASS_CAR, "Rumpo", 3 },
	{ 441, VEHICLE_CLASS_MINI, "RCBandit", 0 },
	{ 442, VEHICLE_CLASS_CAR, "Romero", 1 },
	{ 443, VEHICLE_CLASS_HEAVY, "Packer", 1 },
	{ 444, VEHICLE_CLASS_HEAVY, "Monster", 1 },
	{ 445, VEHICLE_CLASS_CAR, "Admiral", 3 },
	{ 446, VEHICLE_CLASS_BOAT, "Squalo", 0 },
	{ 447, VEHICLE_CLASS_HELI, "Seasparrow", 1 },
	{ 448, VEHICLE_CLASS_BIKE, "PizzaBoy", 0 },

	// needs to be researched to find actual max passengers in SA:MP
	{ 449, VEHICLE_CLASS_HEAVY, "Trolly", 1 },

	/* train... */
	{ 450, VEHICLE_CLASS_TRAILER, "ArtictTrailer2", 0 },
	{ 451, VEHICLE_CLASS_CAR_FAST, "Turismo", 1 },
	{ 452, VEHICLE_CLASS_BOAT, "Speeder", 0 },
	{ 453, VEHICLE_CLASS_BOAT, "Reefer", 0 },
	{ 454, VEHICLE_CLASS_BOAT, "Tropic", 0 },
	{ 455, VEHICLE_CLASS_HEAVY, "Flatbed", 1 },
	{ 456, VEHICLE_CLASS_HEAVY, "Yankee", 1 },
	{ 457, VEHICLE_CLASS_MINI, "Caddy", 1 },
	{ 458, VEHICLE_CLASS_CAR, "Solair", 3 },
	{ 459, VEHICLE_CLASS_HEAVY, "BerkleysRCVan", 3 },
	{ 460, VEHICLE_CLASS_AIRPLANE, "Skimmer", 1 },
	{ 461, VEHICLE_CLASS_BIKE, "PCJ600", 1 },
	{ 462, VEHICLE_CLASS_BIKE, "Faggio", 1 },
	{ 463, VEHICLE_CLASS_BIKE, "Freeway", 1 },
	{ 464, VEHICLE_CLASS_MINI, "RCBaron", 0 },
	{ 465, VEHICLE_CLASS_MINI, "RCRaider", 0 },
	{ 466, VEHICLE_CLASS_CAR, "Glendale", 3 },
	{ 467, VEHICLE_CLASS_CAR, "Oceanic", 3 },
	{ 468, VEHICLE_CLASS_BIKE, "Sanchez", 1 },
	{ 469, VEHICLE_CLASS_HELI, "Sparrow", 1 },
	{ 470, VEHICLE_CLASS_CAR, "Patriot", 3 },
	{ 471, VEHICLE_CLASS_BIKE, "Quadbike", 1 },

	/* sort of.. */
	{ 472, VEHICLE_CLASS_BOAT, "Coastguard", 0 },
	{ 473, VEHICLE_CLASS_BOAT, "Dinghy", 0 },
	{ 474, VEHICLE_CLASS_CAR, "Hermes", 1 },
	{ 475, VEHICLE_CLASS_CAR, "Sabre", 1 },
	{ 476, VEHICLE_CLASS_AIRPLANE, "Rustler", 0 },
	{ 477, VEHICLE_CLASS_CAR_FAST, "ZR350", 1 },
	{ 478, VEHICLE_CLASS_CAR, "Walton", 1 },
	{ 479, VEHICLE_CLASS_CAR, "Regina", 3 },
	{ 480, VEHICLE_CLASS_CAR_FAST, "Comet", 1 },
	{ 481, VEHICLE_CLASS_BIKE, "BMX", 0 },
	{ 482, VEHICLE_CLASS_HEAVY, "Burrito", 3 },

	// more research on this, the side door might allow 2 passengers
	{ 483, VEHICLE_CLASS_HEAVY, "Camper", 2 },
	{ 484, VEHICLE_CLASS_BOAT, "Marquis", 0 },
	{ 485, VEHICLE_CLASS_MINI, "Baggage", 0 },
	{ 486, VEHICLE_CLASS_HEAVY, "Dozer", 0 },
	{ 487, VEHICLE_CLASS_HELI, "Maverick", 3 },
	{ 488, VEHICLE_CLASS_HELI, "NewsChopper", 1 },
	{ 489, VEHICLE_CLASS_CAR, "Rancher", 1 },
	{ 490, VEHICLE_CLASS_CAR, "FBIRancher", 3 },
	{ 491, VEHICLE_CLASS_CAR, "Virgo", 1 },
	{ 492, VEHICLE_CLASS_CAR, "Greenwood", 3 },
	{ 493, VEHICLE_CLASS_BOAT, "Jetmax", 0 },
	{ 494, VEHICLE_CLASS_CAR_FAST, "HotringRacer", 1 },
	{ 495, VEHICLE_CLASS_CAR, "Sandking", 1 },
	{ 496, VEHICLE_CLASS_CAR, "BlistaCompact", 1 },
	{ 497, VEHICLE_CLASS_HELI, "PoliceMaverick", 3 },
	{ 498, VEHICLE_CLASS_HEAVY, "Boxville", 3 },
	{ 499, VEHICLE_CLASS_HEAVY, "Benson", 1 },
	{ 500, VEHICLE_CLASS_CAR, "Mesa", 1 },
	{ 501, VEHICLE_CLASS_MINI, "RCGoblin", 0 },
	{ 502, VEHICLE_CLASS_CAR_FAST, "HotringRacer2", 1 },
	{ 503, VEHICLE_CLASS_CAR_FAST, "HotringRacer3", 1 },
	{ 504, VEHICLE_CLASS_CAR_FAST, "BloodringBanger", 1 },
	{ 505, VEHICLE_CLASS_CAR, "Rancher", 1 },
	{ 506, VEHICLE_CLASS_CAR_FAST, "SuperGT", 1 },
	{ 507, VEHICLE_CLASS_CAR, "Elegant", 3 },
	{ 508, VEHICLE_CLASS_HEAVY, "Journey", 1 },
	{ 509, VEHICLE_CLASS_BIKE, "Bike", 0 },
	{ 510, VEHICLE_CLASS_BIKE, "MountainBike", 0 },
	{ 511, VEHICLE_CLASS_AIRPLANE, "Beagle", 1 },
	{ 512, VEHICLE_CLASS_AIRPLANE, "Cropduster", 0 },
	{ 513, VEHICLE_CLASS_AIRPLANE, "Stuntplane", 0 },
	{ 514, VEHICLE_CLASS_HEAVY, "Tanker", 1 },

	/* semi truck */
	{ 515, VEHICLE_CLASS_HEAVY, "Roadtrain", 1 },

	/* semi truck */
	{ 516, VEHICLE_CLASS_CAR, "Nebula", 3 },
	{ 517, VEHICLE_CLASS_CAR, "Majestic", 1 },
	{ 518, VEHICLE_CLASS_CAR, "Buccaneer", 1 },
	{ 519, VEHICLE_CLASS_AIRPLANE, "Shamal", 0 },
	{ 520, VEHICLE_CLASS_AIRPLANE, "Hydra", 0 },
	{ 521, VEHICLE_CLASS_BIKE, "FCR900", 1 },
	{ 522, VEHICLE_CLASS_BIKE, "NRG500", 1 },
	{ 523, VEHICLE_CLASS_BIKE, "HPV1000", 1 },
	{ 524, VEHICLE_CLASS_HEAVY, "CementTruck", 1 },
	{ 525, VEHICLE_CLASS_HEAVY, "Towtruck", 1 },
	{ 526, VEHICLE_CLASS_CAR, "Fortune", 1 },
	{ 527, VEHICLE_CLASS_CAR, "Cadrona", 1 },
	{ 528, VEHICLE_CLASS_HEAVY, "FBITruck", 1 },
	{ 529, VEHICLE_CLASS_CAR, "Willard", 3 },
	{ 530, VEHICLE_CLASS_MINI, "Forklift", 0 },
	{ 531, VEHICLE_CLASS_CAR, "Tractor", 0 },
	{ 532, VEHICLE_CLASS_HEAVY, "CombineHarvester", 0 },
	{ 533, VEHICLE_CLASS_CAR, "Feltzer", 1 },
	{ 534, VEHICLE_CLASS_CAR, "Remington", 1 },
	{ 535, VEHICLE_CLASS_CAR_FAST, "Slamvan", 1 },
	{ 536, VEHICLE_CLASS_CAR_FAST, "Blade", 1 },
	{ 537, VEHICLE_CLASS_HEAVY, "Freight", 1 },

	/* train engine... */
	{ 538, VEHICLE_CLASS_HEAVY, "BrownStreakEngine", 1 },

	/* train engine... */
	{ 539, VEHICLE_CLASS_BOAT, "Vortex", 0 },
	{ 540, VEHICLE_CLASS_CAR, "Vincent", 3 },
	{ 541, VEHICLE_CLASS_CAR_FAST, "Bullet", 1 },
	{ 542, VEHICLE_CLASS_CAR, "Clover", 1 },
	{ 543, VEHICLE_CLASS_CAR, "Sadler", 1 },
	{ 544, VEHICLE_CLASS_HEAVY, "FireTruckwithladder", 1 },
	{ 545, VEHICLE_CLASS_CAR, "Hustler", 1 },
	{ 546, VEHICLE_CLASS_CAR, "Intruder", 3 },
	{ 547, VEHICLE_CLASS_CAR, "Primo", 3 },
	{ 548, VEHICLE_CLASS_HELI, "Cargobob", 1 },
	{ 549, VEHICLE_CLASS_CAR, "Tampa", 1 },
	{ 550, VEHICLE_CLASS_CAR, "Sunrise", 3 },
	{ 551, VEHICLE_CLASS_CAR, "Merit", 3 },
	{ 552, VEHICLE_CLASS_HEAVY, "UtilityVan", 1 },
	{ 553, VEHICLE_CLASS_AIRPLANE, "Nevada", 0 },
	{ 554, VEHICLE_CLASS_CAR, "Yosemite", 1 },
	{ 555, VEHICLE_CLASS_CAR, "Windsor", 1 },
	{ 556, VEHICLE_CLASS_HEAVY, "Monster2", 1 },
	{ 557, VEHICLE_CLASS_HEAVY, "Monster3", 1 },
	{ 558, VEHICLE_CLASS_CAR_FAST, "Uranus", 1 },
	{ 559, VEHICLE_CLASS_CAR_FAST, "Jester", 1 },
	{ 560, VEHICLE_CLASS_CAR_FAST, "Sultan", 3 },
	{ 561, VEHICLE_CLASS_CAR, "Stratum", 3 },
	{ 562, VEHICLE_CLASS_CAR_FAST, "Elegy", 1 },
	{ 563, VEHICLE_CLASS_HELI, "Raindance", 1 },
	{ 564, VEHICLE_CLASS_MINI, "RCTiger", 0 },
	{ 565, VEHICLE_CLASS_CAR, "Flash", 1 },
	{ 566, VEHICLE_CLASS_CAR, "Tahoma", 3 },
	{ 567, VEHICLE_CLASS_CAR, "Savanna", 3 },
	{ 568, VEHICLE_CLASS_CAR_FAST, "Bandito", 0 },
	{ 569, VEHICLE_CLASS_HEAVY, "FreightTrainFlatbed", 0 },

	/* train car... */
	{ 570, VEHICLE_CLASS_HEAVY, "BrownStreak", 0 },

	/* train car... XXX dupe, streakc */
	{ 571, VEHICLE_CLASS_MINI, "Kart", 0 },
	{ 572, VEHICLE_CLASS_MINI, "Mower", 0 },
	{ 573, VEHICLE_CLASS_HEAVY, "Dune", 1 },
	{ 574, VEHICLE_CLASS_MINI, "Sweeper", 0 },
	{ 575, VEHICLE_CLASS_CAR, "Broadway", 1 },
	{ 576, VEHICLE_CLASS_CAR, "Tornado", 1 },
	{ 577, VEHICLE_CLASS_AIRPLANE, "AT400", 0 },
	{ 578, VEHICLE_CLASS_HEAVY, "DFT30", 1 },

	/* large flat-bed truck */
	{ 579, VEHICLE_CLASS_CAR, "Huntley", 3 },
	{ 580, VEHICLE_CLASS_CAR, "Stafford", 1 },
	{ 581, VEHICLE_CLASS_BIKE, "BF400", 1 },
	{ 582, VEHICLE_CLASS_HEAVY, "NewsVan", 3 },
	{ 583, VEHICLE_CLASS_MINI, "Tug", 0 },
	{ 584, VEHICLE_CLASS_TRAILER, "PetrolTrailer", 0 },
	{ 585, VEHICLE_CLASS_CAR, "Emperor", 3 },
	{ 586, VEHICLE_CLASS_BIKE, "Wayfarer", 1 },
	{ 587, VEHICLE_CLASS_CAR_FAST, "Euros", 1 },
	{ 588, VEHICLE_CLASS_HEAVY, "Hotdog", 1 },
	{ 589, VEHICLE_CLASS_CAR, "Club", 1 },
	{ 590, VEHICLE_CLASS_HEAVY, "FreightTrainBoxcar", 0 },

	/* train car... */
	{ 591, VEHICLE_CLASS_TRAILER, "ArtictTrailer3", 0 },
	{ 592, VEHICLE_CLASS_AIRPLANE, "Andromada", 0 },
	{ 593, VEHICLE_CLASS_AIRPLANE, "Dodo", 1 },
	{ 594, VEHICLE_CLASS_MINI, "RCCam", 0 },
	{ 595, VEHICLE_CLASS_BOAT, "Launch", 0 },
	{ 596, VEHICLE_CLASS_CAR, "PoliceCarLS", 3 },
	{ 597, VEHICLE_CLASS_CAR, "PoliceCarSF", 3 },
	{ 598, VEHICLE_CLASS_CAR, "PoliceCarLV", 3 },
	{ 599, VEHICLE_CLASS_CAR, "PoliceRanger", 1 },
	{ 600, VEHICLE_CLASS_CAR, "Picador", 1 },
	{ 601, VEHICLE_CLASS_HEAVY, "SWAT", 1 },
	{ 602, VEHICLE_CLASS_CAR_FAST, "Alpha", 1 },
	{ 603, VEHICLE_CLASS_CAR_FAST, "Phoenix", 1 },
	{ 604, VEHICLE_CLASS_CAR, "DamagedGlendale", 3 },
	{ 605, VEHICLE_CLASS_CAR, "DamagedSadler", 1 },
	{ 606, VEHICLE_CLASS_TRAILER, "BaggageTrailer", 0 },
	{ 607, VEHICLE_CLASS_TRAILER, "BaggageTrailer2", 0 },
	{ 608, VEHICLE_CLASS_TRAILER, "TugStaircase", 0 },
	{ 609, VEHICLE_CLASS_HEAVY, "BlackBoxville", 3 },
	{ 610, VEHICLE_CLASS_TRAILER, "FarmTrailer", 0 },
	{ 611, VEHICLE_CLASS_TRAILER, "StreetSweeperTrailer", 0 }
};
