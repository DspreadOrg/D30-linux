The device supports updating the XML format of the EMV parameters via TMS. Please note the following points:

1. The format of the xml file provided by dspread cannot be modified, only the tag values in the xml need to be modified.

```
<app><!--2:A0000000031010-->
	<9F06>A0000000031010</9F06> <!--Application Identifier(AID)–terminal   Visa credit or debit-->
	<DF01>00</DF01> <!--Application Selection Indicator-->
	<9F09>0001</9F09> <!--Terminal Application Version Number-->
	<9F1B>00000000</9F1B> <!--Terminal Floor Limit-->
	<9F53>52</9F53><!--Mastercard Merchant category code-->
	<9F66>36C0C000</9F66> <!--Contactless Terminal Transaction Qualifiers-->
	<9F8125>9F3704</9F8125> <!--Contact Default Dynamic Data Authentication Data Object List (DDOL)-->
	<9F8127>00</9F8127> <!--Contact Maximum Target Percentage to be Used for Biased Random Selection-->
	<9F8128>00</9F8128> <!--Contact Target Percentage to Be Used for Random Selection-->
	<9F8129>00000000</9F8129> <!--Contact Threshold Value for Biased random Selection-->
	<9F812A>F06004A800</9F812A> <!--Contact Terminal Action Code Default-->
	<9F812B>0810A80000</9F812B> <!--Contact Terminal Action Code Denial-->
	<9F812C>F06854F800</9F812C> <!--Contact Terminal Action Code Online-->
	<9F928100>05010000</9F928100> <!--Contactless Unknown-->
	<9F928101>00</9F928101> <!--Contactless online/declined when the ODA failed, 0:declined;1:online-->
	<9F8208>003000000000</9F8208> <!--Contactless Amount Limit-->
	<9F8209>000025000100</9F8209> <!--Contactless CVM Amount Limit-->
	<9F820A>000000000000</9F820A> <!--Contactless Terminal Offline Floor Limit-->
</app>
```

2. The name of the XML file cannot be modified, it must be **<mark>emv_profile_tlv.xml</mark>**

3.  Package the emv_profile_TLV.xml file into a zip file.<mark> EMV-CONF.zip</mark>

4. EMV-CONF. zip will be uploaded to the tms system.


