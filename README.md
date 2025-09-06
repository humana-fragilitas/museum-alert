<p align="center">
    <img src="docs/images/museum_alert_logo.svg" alt="drawing" width="350" />
</p>
<h3 align="center">Proof-of-concept for a dual-purpose museum<br>alerting and information system</h3>

___
Comprehensive IoT solution featuring custom Arduino® Nano ESP32 firmware, detailed electrical schematics, scalable AWS cloud infrastructure, and a cross-platform Angular/Electron desktop application. Initially developed as a capstone project for Curtin University's IoT MicroMasters Program (April–July 2023), with full technical implementation finalized following course certification.
___

## Quick Navigation
⚠️ **Please read the [disclaimer](#disclaimer) before using this project.**

| Submodule | Description | Technologies | Links |
|-----------|-------------|------------------|-------------|
| 🔧&nbsp;[**`museum‑alert‑api`**](https://github.com/humana-fragilitas/museum-alert-api/) | AWS CDK project for creating infrastructure, APIs, and configuration artifacts required to deploy both "Museum Alert Sensor (MAS)" device and "Museum Alert Desktop" application | AWS CDK, TypeScript | [Deploy→](https://github.com/humana-fragilitas/museum-alert-api/?tab=readme-ov-file#prerequisites) |
| 📱&nbsp;[**`museum‑alert‑sketch`**](https://github.com/humana-fragilitas/museum-alert-sketch/) | Electrical schematic and firmware for building the "Museum Alert Sensor (MAS)" | C++, Arduino® Nano ESP32, WiFi, BLE | [Build→](https://github.com/humana-fragilitas/museum-alert-sketch/?tab=readme-ov-file#prerequisites) |
| 🖥️&nbsp;[**`museum‑alert‑desktop`**](https://github.com/humana-fragilitas/museum-alert-desktop/) | Cross-platform desktop application for registering, configuring, and testing the "Museum Alert Sensor (MAS)" | Angular, Electron, TypeScript | [Run→](https://github.com/humana-fragilitas/museum-alert-desktop/?tab=readme-ov-file#prerequisites) |

---

## Introduction

A worldwide provisional assessment conducted in March 2021 by UNESCO on data provided by 104,000 museums in 87 countries revealed how public funding reductions affected almost half of cultural institutions surveyed, with some experiencing cuts of up to 40%, worsening an already dramatic crisis caused by the global events that occurred between 2020 and 2021, which resulted in a 70% decrease in attendance and a 40-60% reduction in revenue compared to 2019 [^1]. It has also highlighted how the described phenomenon has primarily endangered key aspects of conservation: security measures and nurturing relations with the public [^2].

Even before the recent crisis, lack of funding and formal organization forced smaller museums and historical heritage sites, often set up as charitable trusts with strict and uncertain budgets, to leave the responsibility of keeping works of art safe and of explaining them to visitors in the bare hands of curators and volunteers, who often are not an adequate force to carry out those tasks, while only the largest cultural institutions financially sustained by mass tourism (e.g.: Uffizi, Louvre), which account for a minimal percentage over the total 104.000 surveyed by UNESCO in the world [^3], have been realistically able to subsidize the procurement, installation and maintenance of traditional, full-fledged alarm systems and physical information panels to both protect and provide information about the collections.

In his still relevant effort to supply comprehensive guidelines aimed at ensuring museum security and surveillance, Liston [^4] clearly acknowledged the occurrence of the depicted scenario and tried to outline priorities accordingly: “Since many museums or cultural institutions are small and might not have a formal protection organization, protection methods and resources begin with primary protection measures. Primary measures are basic measures. The discussion of primary measures does not prevent the use of more complex measures that are available. A fascination for the complex or more modern practice or machine often does not replace the requirement for a basic protection consideration or service.”
Furthermore, it has been widely reported how, with the contributing factor of the gradual reprise of tourism and attendances in museums [^5], lesser funded collections, exhibitions and cultural heritage sites have been facing a shortage of personnel and means apt to guarantee minimum levels of surveillance and guidance of visitors [^6][^7][^8], underlining the need for a progressive, integrated approach to security strengthening and, more broadly, to assistance to visitors.

## What Problem is Being Solved?

The conceptual product proposed in this document, branded as “Museum Alert“, represents a proof-of-concept designed to precisely address the emerging problems outlined above by supplying two key features centered around safety of works of art and attendees' engagement:

- basic security assistance to museum personnel via distance and tripwire barriers to help prevent trespassing of safety space limits (e.g.: touching works of art) and unauthorized access to restricted areas;
- proximity push notifications to visitors' mobile devices with descriptive content about the collections they are experiencing.

![alt text](./docs/images/museum_alert_sensor_detection.png "Museum Alert Sensor (MAS) detection flow")


![alt text](./docs/images/museum_alert_sensor_beacon.png "Museum Alert Sensor (MAS) information broadcasting flow")

Museum Alert is not intended as a replacement for traditional full-fledged alarm systems and does not comply with Italian [^9] and European [^10] regulations on anti-intrusion devices. Instead, it aims to provide a basic monitoring and information system for customers with limited funding.
The product offering includes a plug-and-play, Wi-Fi and Bluetooth-enabled ultrasonic distance barrier with optional backup battery, coupled with a mobile app.

The proposed mobile app (not included in this project; rationale provided below) would be freely available to visitors, who could receive notifications on their mobile devices containing information about works of art or places protected by the barriers whenever they approach a sensor. Professional users would be presented with two service options:

- a three-month free trial, which would allow a maximum of five user accounts (one administrator and four collaborators) and five registered sensors with alert notifications;
- a monthly premium subscription via in-app purchase that would provide access to additional features:
    * up to 20 collaborators, including 5 administrators, and 100 registered sensors;
    * a CMS system for creating content associated with proximity notifications directed to visitors;
    * an editable museum map to visualize active sensors;
    * yearly statistics summarizing registered alert events and attendee engagement.

The Museum Alert Sensor (MAS) offers a streamlined setup process designed for rapid deployment and ease of use. Configuration begins with an organization representative creating a professional account via the desktop application on an internet-connected device. Once authenticated, any unregistered MAS units can be onboarded through a guided four-step process, which includes entering Wi-Fi credentials to connect each sensor to the local network.

Upon successful connection, each sensor is automatically associated with the authenticated user's company device pool. It then initiates secure communication with the Museum Alert broker using MQTT over TLS (MQTTs), while simultaneously broadcasting Bluetooth Low Energy (BLE) beacons to nearby mobile devices. This dual-channel communication enables real-time alert dispatch and proximity-based notifications.

In addition to its role in sensor registration, the desktop application included in the project functions as a comprehensive diagnostic and configuration tool. It enables users to:

- set the BLE-broadcasted Eddystone-URL for each sensor;
- define the minimum distance threshold that triggers alert notifications;
- verify whether alert notifications are being correctly dispatched to the authenticated user's Company topics.

Given these capabilities, the dedicated mobile application for museum visitors was intentionally excluded from the repository. The desktop application alone provides sufficient functionality to validate the technical feasibility of the solution. Furthermore, the broadcasted Eddystone-URL can be easily detected and verified using widely available tools such as the [BeaconScope](https://play.google.com/store/apps/details?id=com.davidgyoungtech.beaconscanner) app on any smartphone.

## Evolution of Security Systems: From Basic Alerting to AIoT Integration

Basic integrated alerting devices, meant as a first aid to surveillance personnel or complement to full-fledged traditional security systems and aimed at discouraging potential vandalism or theft acts, have been developed and recommended for usage in museums and cultural institutions since the Seventies [^11], mainly consisting of plug and play step mats, capacitance alarms, seismic, microwave, infrared and ultrasonic systems, each simply equipped with buzzers and sirens to signal danger events.
Despite the variety of employed technologies, most devices intended for interior areas protection focused on a precise use case, which is still being developed by prominent contemporary AIoT manufacturers as a fundamental computing feature: crossing detection [^12].
Typically, crossing detection is achieved by two strategies, aimed at slightly different security assurance circumstances:

- tripwire barriers are employed to protect spaces in a linear way, where a precise perimeter is identified as threshold;
- distance barriers are employed to protect areas in a nonlinear way, where more broadly an approximate interspace between two or more subjects within a certain angle of observation is seen as a boundary.

Barriers can be either physical or logical: the former being actualized with specific hardware aimed at creating actual observable thresholds (e.g.: a laser tripwire barrier or an ultrasonic distance barrier, as suggested in this project), the latter being created via software, where the output of a multipurpose or more generic sensor (e.g.: a video camera) is consumed and processed by configurable algorithms able to abstract the information needed to enforce determined spatial boundaries, frequently leveraging the edge computing pattern.

The employment of the technologies known as IoT (Internet of Things) in relation to alarm and surveillance systems, consisting of sensors, actuators, processing ability and software meant to connect and exchange data with other devices over the Internet and other communication networks, has been discussed in the academic literature [^13] since the rise of “connected things” in 2017 [^14], when a 31% percent global increase in their usage compared to the previous year marked its entry in the mainstream business.

Furthermore, the established market of security systems has been investing in software development [^15], particularly embedded AI and cloud services, using open-source artificial intelligence algorithms and libraries, with the most prominent multinational actors in this field on the front line; while this trend may constitute a barrier for our project, it also indirectly defined a new niche market opportunity for low-priced devices aimed at businesses with lesser elaborate requirements.

## Proposed Approach: Dual-Purpose Security and Information System

With the advent of highly accessible and cost-effective microcontroller units (MCUs) — notably Arduino® (2008) and Raspberry Pi® (2011) — specifically designed to facilitate rapid prototyping of electronic systems, a robust ecosystem of companion devices, sensors, actuators, extensive online documentation, open-source software, and publicly available projects has emerged. These resources empower entrepreneurs and developers to concentrate on solving real-world customer problems during the product discovery phase, rather than allocating significant time and financial resources to foundational, boilerplate components required for experimentation and feedback collection.

Museum Alert was conceived as an Arduino-based IoT solution that capitalizes on this ecosystem to fulfill its core objective: delivering a low-cost, streamlined, and innovative alerting and information system. The system is designed to support cultural institution staff in surveillance tasks while simultaneously enhancing the visitor experience through interactive proximity-based notifications on mobile devices.

The target audience for Museum Alert includes small museums, art galleries, and cultural heritage sites whose operational needs do not justify the deployment of traditional, high-cost, full-scale alarm systems. Instead, these institutions benefit from a simpler, reliable solution that enforces basic distancing protocols and provides attendees with contextual information about the exhibited works of art.

Museum Alert addresses two key challenges faced by small and medium-sized cultural institutions in the domains of conservation and visitor engagement:

- protection of exhibited pieces and restricted areas;
- information delivery to enhance visitor interaction with collections.

The first challenge has been explored through a review of relevant literature and documentation on contemporary alerting and alarm systems. Focusing on indoor environments, which are central to this research, two primary security scenarios have been identified:

- protection of linear spaces—such as corridors with artworks displayed along walls, where linear barriers are used to maintain distance;
- designation of restricted zones—nonlinear areas requiring protection based on proximity to a specific point and the angle of observation by attendees.

Deployment of linear barriers is widely documented [^16] with frequent references to the usage of photoelectric (IR and non-IR) tripwire alert systems where a physical interruption between the beam emitted by a laser diode and a corresponding light-dependent resistor (LDR) causes a reaction in a physical or logical actuator [^17]. Safety observations in relation to the implementation of such means have been considered accordingly drawing information from the healthcare scientific literature [^18].
Further studies and experiments describing the employment of light (laser or IR) beams in the museum security industry outline drawbacks which would not dramatically discourage its employment in relation to the use case illustrated in this document: “Problem areas for this system, which might cause nuisance alarms, are sunlight, animals and radiators. Some of these problems can be solved by not aiming the sensor directly at the radiator or into the direct rays of the sun.” [^19]
Alternatives such as physical barriers (e.g.: step mats) and line crossing detection via video cameras have been respectively excluded from the project scope due to:

- the necessity of avoiding invasiveness of security devices in places often subject to heritage protection regulations which forbid any visible and substantial modifications to existing furnishings and environments;
- marketing and product placement considerations: Museum Alert aims at marginally competing with advanced and expensive AI-enabled video security systems by filling the lower market niche with a basic low-cost yet efficient alerting device.

Similarly, ultrasonic devices are historically documented as broadly deployed in instances where intrusion detection based on a minimum distance from an area defined by and angle of observation is needed [^20] and have been generally reviewed in the scientific literature as a founded technological choice in relation to the use cases described in this document [^21][^22][^23]. Furthermore, ultrasonic sensors are, for the most part, completely insensitive to hindering factors like light, dust, smoke, mist, vapor and lint. 
Bluetooth Low Energy beacons are the current de facto standard for creating marketing and information proximity push notification flows and have been chosen also bearing in mind potential future developments including tools to help visually impaired visitors, who could greatly receive help from such automatism.

The unique value proposition and novel technical approach of Museum Alert resides in its dual purpose: basic alerting system for museum security personnel and proximity information system for museum visitors in a single low-priced device.

## Network and Proximity Communication

A technical assessment of available Internet connectivity options for sensor devices, based on their intended use cases, identified Wi-Fi as the most viable and practical solution. This conclusion reflects both manufacturing considerations and end-user experience: Wi-Fi networks are already widely deployed across museums and cultural institutions [^24], support the WPA3™ security standard, and offer a cost-effective, easily configurable infrastructure.

In parallel, Bluetooth Low Energy (BLE) beacons have been extensively documented in the scientific literature as tools to enrich visitor engagement in art exhibitions—through mechanisms such as gamification [^25] and interactive collection displays [^26]. The underlying BLE technology is well supported by mainstream prototyping microcontrollers, including the Arduino® Nano ESP32 used in this project.

## Disclaimer

### Important Notice

This open source project, including all its submodules, documentation, and associated code (collectively, the "Project"), is provided for educational and experimental purposes only.

### No Warranty

THE PROJECT IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. THE AUTHOR MAKES NO WARRANTIES ABOUT THE ACCURACY, RELIABILITY, COMPLETENESS, OR TIMELINESS OF THE PROJECT OR ITS COMPONENTS.

### Limitation of Liability

IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE PROJECT OR THE USE OR OTHER DEALINGS IN THE PROJECT. THIS INCLUDES, BUT IS NOT LIMITED TO:

- **AWS Costs**: any charges incurred from AWS services deployed using the provided CDK templates;
- **Hardware Damage**: damage to Arduino® boards, sensors, or other electronic components;
- **Data Loss**: loss of data or configuration settings;
- **Service Interruptions**: downtime or interruptions to connected services;
- **Security Issues**: any security vulnerabilities or breaches;
- **Indirect Damages**: lost profits, business interruption, or consequential damages of any kind.

### User Responsibility

By using this Project, you acknowledge and agree that:

1. **you use the Project entirely at your own risk**;
2. **you are responsible for understanding AWS pricing** and monitoring your usage to avoid unexpected charges;
3. **you should implement appropriate security measures** for any production deployments;
4. **you are responsible for compliance** with all applicable laws and regulations in your jurisdiction;
5. **you should test thoroughly** in development environments before any production use;
6. **you are responsible for backing up** any important data or configurations.

### AWS Specific Notice

This project may create AWS resources that incur charges; users are solely responsible for:
- understanding AWS pricing models;
- monitoring their AWS usage and costs;
- properly terminating or deleting resources when no longer needed;
- reviewing and understanding all CloudFormation templates before deployment.

### Third-Party Components

This Project may include or reference third-party libraries, services, or components. The author is not responsible for the functionality, security, or licensing of these third-party components. Users should review and comply with all applicable third-party licenses and terms of service.

### Modification and Distribution

Users may modify and distribute this Project under the terms of the applicable open source license. However, any modifications or distributions must include this disclaimer, and the author bears no responsibility for modified versions of the Project.

### Professional Advice

This Project is not intended to replace professional consultation. For production systems or critical applications, please consult with qualified professionals in the relevant fields.

### Acknowledgments

By downloading, cloning, forking, or otherwise using this Project, you acknowledge that you have read, understood, and agree to be bound by this disclaimer.

The [museum-alert-desktop](https://github.com/humana-fragilitas/museum-alert-desktop) project has been derived from Maxime Gris's [angular-electron](https://github.com/maximegris/angular-electron) starter project (see the license for further information).

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
