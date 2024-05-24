<p align="center">
    <img src="docs/images/museum_alert_logo.svg" alt="drawing" width="350" />
</p>
<h3 align="center">Proposal for a dual-purpose museum<br>alerting and information system</h3>

___
##### Complete project comprising Arduino ESP32 Nano sketch, Node.js backend APIs and Ionic client mobile app as described in the term paper submitted as IoT Capstone exam final report at Curtin University, Internet of Things (IoT) MicroMasters® Program (April - July 2023).
___

## Introduction

A worldwide provisional assessment conducted last March 2023 by UNESCO on data provided by 104.000 museums in 87 countries has revealed how public funding for cultural institutions has dropped by an average of 40% in the last three years, worsening an already dramatic crisis caused by the global events occurred between 2020 and 2021, which resulted in a 70% decrease in attendance and a 40-60% reduction in revenue compared to 2019 [^1]. It has also highlighted how the described phenomenon has primarily endangered key aspects of conservation: security measures and nurturing relations with the public [^2].

Even before the recent crisis, lack of funding and formal organization forced smaller museums and historical heritage sites, often set up as charitable trusts with strict and uncertain budgets, to leave the responsibility of keeping works of art safe and of explaining them to visitors in the bare hands of curators and volunteers, who often are not an adequate force to carry out those tasks, while only the largest cultural institutions financially sustained by mass tourism (e.g.: Uffizi, Louvre), which account for a minimal percentage over the total 104.000 surveyed by UNESCO in the world [^3], have been realistically able to subsidize the procurement, installation and maintenance of traditional, full-fledged alarm systems and physical information panels to both protect and provide information about the collections.

In his still relevant effort to supply comprehensive guidelines aimed at ensuring museum security and surveillance, Liston [^4] clearly acknowledged the occurrence of the depicted scenario and tried to outline priorities accordingly: “Since many museums or cultural institutions are small and might not have a formal protection organization, protection methods and resources begin with primary protection measures. Primary measures are basic measures. The discussion of primary measures does not prevent the use of more complex measures that are available. A fascination for the complex or more modern practice or machine often does not replace the requirement for a basic protection consideration or service.”
Furthermore, it has been widely reported how, with the contributing factor of the gradual reprise of tourism and attendances in museums [^5], lesser funded collections, exhibitions and cultural heritage sites have been facing a shortage of personnel and means apt to guarantee minimum levels of surveillance and guidance of visitors [^6][^7][^8], underlining the need for a progressive, integrated approach to security strengthening and, more broadly, to assistance to visitors.

## What problem is being solved?

The product proposed in this document, branded as “Museum Alert”, has been conceived to precisely address the emerging problems outlined above by supplying two key features centered around safety of works of art and attendees' engagement:

- basic surveillance aid to museum personnel via distance and tripwire barriers to help to prevent that certain safety space limits are not trespassed (e.g.: works of art are not being touched) and areas restricted from the public are not improperly visited;
- proximity push notifications to visitors’ mobile devices with descriptive content about the collections they are experiencing.

“Museum Alert” is not intended as replacement of a traditional full-fledged alarm system and is not compliant with the Italian [^9] and European [^10] regulations on anti-intrusion devices but aims to supply a basic surveillance and information system to the benefit of customers with limited funding.
Product offering includes two plug and play, Wi-Fi and Bluetooth enabled sensors, a laser tripwire barrier and an ultrasonic distance barrier, with an optional backup battery lasting approximately two hours in case of power outage, coupled with a mobile app hydrated by a backend hosted in the cloud.

The mobile app is freely available to visitors, who can receive notifications on their mobile devices containing information about the works of art or places protected by the barriers themselves whenever they come close to a sensor, while professional users are presented with two choices:
- experience a three-month free trial, which allows a maximum of five user accounts (one administrator and four collaborators) and five registered sensors with alert notifications;
- pay a monthly 40€ premium fee via in-app purchase to access the following additional features:
    * up to 20 collaborators, including 5 administrators, and 100 registered sensors;
    * a CMS system suitable for creating content associated to proximity notifications directed to visitors;
    * an editable museum map to visualize active sensors;
    * yearly statistics summarizing registered alert events and attendee’s engagement.

“Museum Alert” sensors can be set up and configured in two effortless steps.
First, a representative of an organization creates an administrator account using the app installed on a mobile device connected to the local Wi-Fi network; then, any unregistered sensors will be automatically discovered via Bluetooth by the mobile app itself and user will be prompted to enter Wi-Fi credentials to connect them to the same network as the mobile device.
As soon as a sensor is connected to the Internet, it is registered to the authenticated user’s dashboard who initiated the procedure, and it starts communicating with “Museum Alert” broker via MQTTs and with any nearby mobile devices via Bluetooth Low Energy beacons, ready to respectively dispatch alert and proximity notifications.

## Literature review and market research

Basic integrated alerting devices, meant as a first aid to surveillance personnel or complement to full-fledged traditional security systems and aimed at discouraging potential vandalism or theft acts, have been developed and recommended for usage in museums and cultural institutions since the Seventies [^11], mainly consisting of plug and play step mats, capacitance alarms, seismic, microwave, infrared and ultrasonic systems, each simply equipped with buzzers and sirens to signal danger events.
Despite the variety of employed technologies, most devices intended for interior areas protection focused on a precise use case, which is still being developed by prominent contemporary AIoT manufacturers as a fundamental computing feature: crossing detection [^12].
Typically, crossing detection is achieved by two strategies, aimed at slightly different security assurance circumstances:

- tripwire barriers are employed to protect spaces in a linear way, where a precise perimeter is identified as threshold;
- distance barriers are employed to protect areas in a nonlinear way, where more broadly an approximate interspace between two or more subjects within a certain angle of observation is seen as a boundary.

Barriers can be either physical or logical: the former being actualized with specific hardware aimed at creating actual observable thresholds (e.g.: a laser tripwire barrier or an ultrasonic distance barrier, as suggested in this project), the latter being created via software, where the output of a multipurpose or more generic sensor (e.g.: a video camera) is consumed and processed by configurable algorithms able to abstract the information needed to enforce determined spatial boundaries, frequently leveraging the edge computing pattern.

The employment of the technologies known as IoT (Internet of Things) in relation to alarm and surveillance systems, consisting of sensors, actuators, processing ability and software meant to connect and exchange data with other devices over the Internet and other communication networks, has been discussed in the academic literature [^13] since the rise of “connected things” in 2017 [^14], when a 31% percent global increase in their usage compared to the previous year marked its entry in the mainstream business.

Furthermore, the established market of security systems has been investments towards software development [^15], particularly embedded AI and cloud services, using open-source artificial intelligence algorithms and libraries, with the four most prominent actors in this field, Artsentry, Honeywell, Dallmeier and Hikvision on the front line; while this trend may constitute a barrier for our project, it also indirectly defined a new niche market opportunity for low-priced devices aimed at businesses with lesser elaborate requirements.

## Evalutation of alternative solutions

With the introduction of highly usable and affordable MCUs (Micro Controller Units), mainly Arduino (2008) and Raspberry PI (2011), precisely intended for easing the task of rapidly prototyping electronic systems, an ecosystem of advantageous yet efficient companion devices, sensors, actuators, online documentation, open-source software and publicly available projects has been created to allow entrepreneurs to mainly focus on solving customers’ problems in the product discovery phase, rather than investing time and financial resources on boilerplate, foundational custom components needed to experiment and gather feedback. “Museum Alert” has been conceived as an Arduino-based IoT project to leverage the convenience of such resources and primarily achieve its main goal: to supply a low-cost, basic yet innovative alerting and information system dedicated to both helping cultural institutions personnel in surveillance tasks and enhancing visitors’ experience with interactive proximity notifications on mobile devices.

“Museum Alert” target customers are small museums, art galleries, cultural heritage sites whose needs do not require a traditional, massively expensive, full-fledged alarm system but a simpler, robust solution to simply enforce basic distancing rules while at the same time providing attendees with valuable information about the exhibited works of art to enhance their experience.


"Museum Alert” works towards solving two problems related to works of art conservation and enhancement in small and medium-sized museums and cultural institutions:

- protection of exhibited pieces or restricted areas;
- information provisioning about the collections to engage and support attendees.

The former issue has been addressed by investigating the related use cases with the aid of the available literature and documentation concerning contemporary alerting and alarm systems; in particular, with reference to indoor spaces, which form the focus of our research, two main security occurrences have been identified:

- protection of linear spaces with similarly linear barriers, e.g.: a corridor with paintings or sculptures hanged on walls;
- designation of restricted areas, where a nonlinear space is expected to be protected based on distance from a certain point and attendees with a determined angle of observation.

Deployment of linear barriers is widely documented [^16] with frequent references to the usage of photoelectric (IR and non-IR) tripwire alert systems where a physical interruption between the beam emitted by a laser diode and a corresponding light-dependent resistor (LDR) causes a reaction in a physical or logical actuator [^17]. Safety observations in relation to the implementation of such means have been considered accordingly drawing information from the healthcare scientific literature [^18].
Further studies and experiments describing the employment of light (laser or IR) beams in the museum security industry outline drawbacks which would not dramatically discourage its employment in relation to the use case illustrated in this document: “Problem areas for this system, which might cause nuisance alarms, are sunlight, animals and radiators. Some of these problems can be solved by not aiming the sensor directly at the radiator or into the direct rays of the sun.” [^19]
Alternatives such as physical barriers (e.g.: step mats) and line crossing detection via video cameras have been respectively excluded from the project scope due to:

- the necessity of avoiding invasiveness of security devices in places often subject to monumental bonds which forbid any visible and substantial modifications to existing furnishings and environments;
- marketing and product placement considerations: “Museum Alert” aims at marginally competing with advanced and expensive AI-enabled video security systems by filling the lower market niche with a basic low-cost yet efficient alerting device.

Similarly, ultrasonic devices are historically documented as broadly deployed in instances where intrusion detection based on a minimum distance from an area defined by and angle of observation is needed [^20] and have been generally reviewed in the scientific literature as a founded technological choice in relation to the use cases described in this document [^21][^22][^23]. Furthermore, ultrasonic sensors are, for the most part, completely insensitive to hindering factors like light, dust, smoke, mist, vapor and lint. 
Bluetooth Low Energy beacons are the current de facto standard for creating marketing and information proximity push notification flows and have been chosen also bearing in mind potential future developments including tools to help visually impaired visitors, who could greatly receive help from such automatism.

The unique value proposition and novel technical approach of “Museum Alert” resides in its dual purpose: basic alerting system for museum security personnel and proximity information system for museum visitors in a single low-priced device.

## Specifications
### Ultrasonic distance sensor

- input voltage: 12V dc with 5.5/2.1mm low voltage connector with 12V backup battery;
- working temperature: -10°C ~ 40°C;
- maximum barrier range: 2cm ~ 400cm;
- measuring angle: <15°;
- operating frequency: 40KHz;
- maximum dimensions: 14cm x 8cm x 4cm;
- supported Wi-Fi standards:
    * Wi‑Fi 6 (IEEE 802.11ax): 600–9608 Mbit/s; 2,4/5 GHz (1–6 GHz ISM);
    * Wi‑Fi 5 (IEEE 802.11ac): 433–6933 Mbit/s; 5 GHz;
- supported Bluetooth (including BLE) implementations:
    * Bluetooth 5.1 - 5.4 (range: 0 - 10m);
- EEPROM storage size: 256 bytes;
- not suitable for outdoor usage.

A technical investigation on available Internet connectivity options for the sensor devices in relation to their use cases concluded that Wi-Fi is the most workable and practical solution, both from a manufacturing and end users’ experience standpoint: Wi-Fi enabled networks are already available in the vast majority of museums and cultural institutions [^24], implement WPA3™ security standard and are relatively low-priced and easy to set up.

Also, in the context of art exhibitions, Bluetooth Low Energy beacons have been documented in the scientific literature as means to enhance the overall experience of visitors via gamification [^25] and illustration of collections [^26] and the underlying technology is well supported by common prototyping MCUs such as Arduino UNO Wi-Fi REV2 used in this project.

[^1]: UNESCO, “Supporting museums: UNESCO report points to options for the future,” UNESCO, 2023. [Online]. Available: https://www.unesco.org/. [Accessed: June 1, 2023].
[^2]: UNESCO, “UNESCO report: museums around the world in the face of COVID-19,” UNESCO, CLT/CCE/2021/RP/1, 2021. [Online]. Available: https://unesdoc.unesco.org/. [Accessed: June 1, 2023].
[^3]: Statista Research Department, “Number of museums worldwide as of March 2021, by UNESCO regional classification,” Statista, 2023. [Online]. Available: https://www.statista.com/. [Accessed: June 5, 2023].
[^4]: D. Liston, Ed., Museum Security and Protection. A handbook for cultural heritage institutions, London: Routledge, 1993, p. 16.
[^5]: L. Cheshire, J. da Silva. Research conducted by A. Colville, J. Kamp, “Visitor figures 2022,” The Art Newspaper, 2023. [Online]. Available: https://www.theartnewspaper.com/. [Accessed: June 5, 2023].
[^6]: L. Gorini, “Why Italian museums are suffering from an unusual shortage of staff,” NSS Magazine, 2023. [Online]. Available: https://www.nssmag.com/. [Accessed: June 5, 2023]. 
[^7]: P. McGlone, “Security officers say Smithsonian staff shortages threaten safety of priceless treasures, and people,” The Washington Post, January 14, 2022. [Online]. Available: https://www.washingtonpost.com/. [Accessed: June 5, 2023].
[^8]: H. Lyons, “Museum security shortage means many masterpieces closed to public,” The Brussels Times, March 26, 2021. [Online]. Available: https://www.brusselstimes.com/. [Accessed: June 5, 2023].
[^9]: S. Berri, “Norma CEI 79-3: impianti antintrusione a regola d’arte,” Elettronews, December 2019. [Online]. Available: https://www.elettronews.com/. [Accessed June 6, 2023].
[^10]: CEI-CT79 committee, “CEI EN 50131-1/A3”, CEI - Comitato Elettrotecnico Italiano, April 2021. [Online]. Available: https://mycatalogo.ceinorme.it/. [Accessed June 6, 2023].
[^11]: D. L. Mason, The fine art of art security: protecting public and private collections against theft, fire, and vandalism, New York: Van Nostrand Reinhold Co., 1979, p. 73.
[^12]: U. Guterman, “Top Five Video Surveillance Trends for 2022”, Global Security Mag, 2022. [Online]. Available: https://www.globalsecuritymag.com/. [Accessed: 13 June, 2023]
[^13]: C. Chilipirea, A. Ursache, D. Octavian Popa and F. Pop, “Energy efficiency and robustness for IoT: Building a smart home security system,” 2016 IEEE 12th International Conference on Intelligent Computer Communication and Processing (ICCP), 2016, pp. 43 – 48. [Online]. Available: https://ieeexplore.ieee.org/. [Accessed: June 12, 2023].
[^14]: R. van der M., “Gartner says 8.4 billion connected ‘things’ will be in use in 2017, Up 31 Percent From 2016”, Gartner, February 7, 2017. [Online]. Available: https://www.gartner.com/. [Accessed June 12, 2023].
[^15]: Source Security, “Hikvision reviews top 8 trends for the security industry in 2022,” Source Security, 2022. [Online]. Available: https://www.sourcesecurity.com/. [Accessed June 22, 2023].
[^16]: D. Liston, Ed., Museum Security and Protection. A handbook for cultural heritage institutions, London: Routledge, 1993, p. 71.
[^17]: M. A. Zainal Arifin, M. Kassim, A. R. Mahmud and S. Izwan Suliman, “Automation Security System with Laser Lights Alarm on Web Pages and Mobile Apps,” 2019 IEEE 9th Symposium on Computer Applications & Industrial Electronics (ISCAIE), 2019, pp. 287-292. [Online]. Available: https://ieeexplore.ieee.org/. [Accessed June 22, 2023]. 
[^18]: Smalley P. J., “Laser safety: Risks, hazards, and control measures,” Laser therapy, vol. 20 (2), 2011, pp. 95–106. [Online]. Available: https://www.ncbi.nlm.nih.gov/. [Accessed June 22, 2023].
[^19]: D. L. Mason, The fine art of art security: protecting public and private collections against theft, fire, and vandalism, New York: Van Nostrand Reinhold Co., 1979, p. 35.
[^20]: D. L. Mason, The fine art of art security: protecting public and private collections against theft, fire, and vandalism, New York: Van Nostrand Reinhold Co., 1979, pp. 73.
[^21]: U. Sayin, D. Scaini and D. Arteaga, “Volumetric Security Alarm Based on a Spherical Ultrasonic Transducer Array,” Physics Procedia, vol. 70, 2015, pp. 876-879. [Online]. Available: https://www.sciencedirect.com. [Accessed June 21, 2023].
[^22]: P. Janů and B. Odvárková, “An intelligent barrier using ultrasonic technology,” Journal of Electrical Engineering, vol. 73 (5), 2022, pp. 343-349. [Online]. Available: https://sciendo.com/. [Accessed June 21, 2023].
[^23]: C. Loughlin, “Ultrasonic measurement: keeping your distance,” Sensor Review, vol. 9 (2), 1989, pp. 85-89. [Online]. Available: https://www.emerald.com/. [Accessed June 21, 2023]. 
[^24]: C. Hetting, “Museums: A growth market for Wi-Fi solution providers,” Wi-Fi Now, 2017. [Online]. Available: https://wifinowglobal.com/. [Accessed June 22, 2023].
[^25]: T. Nilsson, A. F. Blackwell, C. Hogsden, and David Scruton, “Ghosts! A Location-Based Bluetooth LE Mobile Game for Museum Exploration,” Mapping the Digital: Cultures and Territories of Play, 2016, pp. 129-138. [Online]. Available: https://brill.com/. [Accessed June 22, 2023].
[^26]: P. Barsocchi, M. Girolami, D. La Rosa, “Detecting Proximity with Bluetooth Low Energy Beacons for Cultural Heritage,” Sensors (21), 2021, pp. 1-24. [Online]. Available: https://www.mdpi.com/. [Accessed June 22, 2023].
