// FastNoise.cpp
//
// MIT License
//
// Copyright(c) 2016 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

#include "FastNoise.h"
#include <math.h>
#include <assert.h>
#include <random>

const float GRAD_X[] =
{
	1, -1, 1, -1,
	1, -1, 1, -1,
	0, 0, 0, 0
};
const float GRAD_Y[] =
{
	1, 1, -1, -1,
	0, 0, 0, 0,
	1, -1, 1, -1
};
const float GRAD_Z[] =
{
	0, 0, 0, 0,
	1, 1, -1, -1,
	1, 1, -1, -1
};

const float GRAD_4D[] =
{
	0,1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,
	0,-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,
	1,0,1,1,1,0,1,-1,1,0,-1,1,1,0,-1,-1,
	-1,0,1,1,-1,0,1,-1,-1,0,-1,1,-1,0,-1,-1,
	1,1,0,1,1,1,0,-1,1,-1,0,1,1,-1,0,-1,
	-1,1,0,1,-1,1,0,-1,-1,-1,0,1,-1,-1,0,-1,
	1,1,1,0,1,1,-1,0,1,-1,1,0,1,-1,-1,0,
	-1,1,1,0,-1,1,-1,0,-1,-1,1,0,-1,-1,-1,0
};

const float VAL_LUT[] =
{
	0.3490196078f, 0.4352941176f, -0.4509803922f, 0.6392156863f, 0.5843137255f, -0.1215686275f, 0.7176470588f, -0.1058823529f, 0.3960784314f, 0.0431372549f, -0.03529411765f, 0.3176470588f, 0.7254901961f, 0.137254902f, 0.8588235294f, -0.8196078431f,
	-0.7960784314f, -0.3333333333f, -0.6705882353f, -0.3882352941f, 0.262745098f, 0.3254901961f, -0.6470588235f, -0.9215686275f, -0.5294117647f, 0.5294117647f, -0.4666666667f, 0.8117647059f, 0.3803921569f, 0.662745098f, 0.03529411765f, -0.6156862745f,
	-0.01960784314f, -0.3568627451f, -0.09019607843f, 0.7490196078f, 0.8352941176f, -0.4039215686f, -0.7490196078f, 0.9529411765f, -0.0431372549f, -0.9294117647f, -0.6549019608f, 0.9215686275f, -0.06666666667f, -0.4431372549f, 0.4117647059f, -0.4196078431f,
	-0.7176470588f, -0.8117647059f, -0.2549019608f, 0.4901960784f, 0.9137254902f, 0.7882352941f, -1.0f, -0.4745098039f, 0.7960784314f, 0.8509803922f, -0.6784313725f, 0.4588235294f, 1.0f, -0.1843137255f, 0.4509803922f, 0.1450980392f,
	-0.231372549f, -0.968627451f, -0.8588235294f, 0.4274509804f, 0.003921568627f, -0.003921568627f, 0.2156862745f, 0.5058823529f, 0.7647058824f, 0.2078431373f, -0.5921568627f, 0.5764705882f, -0.1921568627f, -0.937254902f, 0.08235294118f, -0.08235294118f,
	0.9058823529f, 0.8274509804f, 0.02745098039f, -0.168627451f, -0.7803921569f, 0.1137254902f, -0.9450980392f, 0.2f, 0.01960784314f, 0.5607843137f, 0.2705882353f, 0.4431372549f, -0.9607843137f, 0.6156862745f, 0.9294117647f, -0.07450980392f,
	0.3098039216f, 0.9921568627f, -0.9137254902f, -0.2941176471f, -0.3411764706f, -0.6235294118f, -0.7647058824f, -0.8901960784f, 0.05882352941f, 0.2392156863f, 0.7333333333f, 0.6549019608f, 0.2470588235f, 0.231372549f, -0.3960784314f, -0.05098039216f,
	-0.2235294118f, -0.3725490196f, 0.6235294118f, 0.7019607843f, -0.8274509804f, 0.4196078431f, 0.07450980392f, 0.8666666667f, -0.537254902f, -0.5058823529f, -0.8039215686f, 0.09019607843f, -0.4823529412f, 0.6705882353f, -0.7882352941f, 0.09803921569f,
	-0.6078431373f, 0.8039215686f, -0.6f, -0.3254901961f, -0.4117647059f, -0.01176470588f, 0.4823529412f, 0.168627451f, 0.8745098039f, -0.3647058824f, -0.1607843137f, 0.568627451f, -0.9921568627f, 0.9450980392f, 0.5137254902f, 0.01176470588f,
	-0.1450980392f, -0.5529411765f, -0.5764705882f, -0.1137254902f, 0.5215686275f, 0.1607843137f, 0.3725490196f, -0.2f, -0.7254901961f, 0.631372549f, 0.7098039216f, -0.568627451f, 0.1294117647f, -0.3098039216f, 0.7411764706f, -0.8509803922f,
	0.2549019608f, -0.6392156863f, -0.5607843137f, -0.3176470588f, 0.937254902f, 0.9843137255f, 0.5921568627f, 0.6941176471f, 0.2862745098f, -0.5215686275f, 0.1764705882f, 0.537254902f, -0.4901960784f, -0.4588235294f, -0.2078431373f, -0.2156862745f,
	0.7725490196f, 0.3647058824f, -0.2392156863f, 0.2784313725f, -0.8823529412f, 0.8980392157f, 0.1215686275f, 0.1058823529f, -0.8745098039f, -0.9843137255f, -0.7019607843f, 0.9607843137f, 0.2941176471f, 0.3411764706f, 0.1529411765f, 0.06666666667f,
	-0.9764705882f, 0.3019607843f, 0.6470588235f, -0.5843137255f, 0.05098039216f, -0.5137254902f, -0.137254902f, 0.3882352941f, -0.262745098f, -0.3019607843f, -0.1764705882f, -0.7568627451f, 0.1843137255f, -0.5450980392f, -0.4980392157f, -0.2784313725f,
	-0.9529411765f, -0.09803921569f, 0.8901960784f, -0.2862745098f, -0.3803921569f, 0.5529411765f, 0.7803921569f, -0.8352941176f, 0.6862745098f, 0.7568627451f, 0.4980392157f, -0.6862745098f, -0.8980392157f, -0.7725490196f, -0.7098039216f, -0.2470588235f,
	-0.9058823529f, 0.9764705882f, 0.1921568627f, 0.8431372549f, -0.05882352941f, 0.3568627451f, 0.6078431373f, 0.5450980392f, 0.4039215686f, -0.7333333333f, -0.4274509804f, 0.6f, 0.6784313725f, -0.631372549f, -0.02745098039f, -0.1294117647f,
	0.3333333333f, -0.8431372549f, 0.2235294118f, -0.3490196078f, -0.6941176471f, 0.8823529412f, 0.4745098039f, 0.4666666667f, -0.7411764706f, -0.2705882353f, 0.968627451f, 0.8196078431f, -0.662745098f, -0.4352941176f, -0.8666666667f, -0.1529411765f,
};

const float CELL_2D_X[] =
{
	-0.4313539279f, -0.1733316799f, -0.2821957395f, -0.2806473808f, 0.3125508975f, 0.3383018443f, -0.4393982022f, -0.4460443703f, -0.302223039f, -0.212681052f, -0.2991156529f, 0.2293323691f, 0.4475439151f, 0.1777518f, 0.1688522499f, -0.0976597166f,
	0.08450188373f, -0.4098760448f, 0.3476585782f, -0.3350670039f, 0.2298190031f, -0.01069924099f, -0.4460141246f, 0.3650293864f, -0.349479423f, -0.4122720642f, -0.267327811f, 0.322124041f, 0.2880445931f, 0.3892170926f, 0.4492085018f, -0.4497724772f,
	0.1278175387f, -0.03572100503f, -0.4297407068f, -0.3217817723f, -0.3057158873f, -0.414503978f, -0.3738139881f, 0.2236891408f, 0.002967775577f, 0.1747128327f, -0.4423772489f, -0.2763960987f, -0.4019385906f, 0.3871414161f, -0.430008727f, -0.03037574274f,
	-0.3486181573f, 0.04553517144f, -0.0375802926f, 0.3266408905f, 0.06540017593f, 0.03409025829f, -0.4449193635f, -0.4255936157f, 0.449917292f, 0.05242606404f, -0.4495305179f, -0.1204775703f, -0.341986385f, 0.3865320182f, 0.04506097811f, -0.06283465979f,
	0.3932600341f, 0.4472261803f, 0.3753571011f, -0.273662295f, 0.1700461538f, 0.4102692229f, 0.323227187f, -0.2882310238f, 0.2050972664f, 0.4414085979f, -0.1684700334f, -0.003978032396f, -0.2055133639f, -0.006095674897f, -0.1196228124f, 0.3901528491f,
	0.01723531752f, -0.3015070339f, -0.01514262423f, -0.4142574071f, -0.1916377265f, 0.3749248747f, -0.2237774255f, -0.4166343106f, 0.3619171625f, 0.1891126846f, -0.3127425077f, -0.3281807787f, -0.2294806661f, -0.3445266136f, -0.4167095422f, -0.257890321f,
	-0.3612037825f, 0.2267996491f, 0.207157062f, 0.08355176718f, -0.4312233307f, 0.3257055497f, 0.177701095f, -0.445182522f, 0.3955143435f, -0.4264613988f, -0.3793799665f, 0.04617599081f, -0.371405428f, 0.2563570295f, 0.03476646309f, -0.3065454405f,
	-0.2256979823f, 0.4116448463f, -0.2907745828f, 0.2842278468f, 0.3114589359f, 0.4464155859f, -0.3037334033f, 0.4079607166f, -0.3486948919f, 0.3264821436f, 0.3211142406f, 0.01183382662f, 0.4333844092f, 0.3118668416f, -0.272753471f, -0.422228622f,
	-0.1009700099f, -0.2741171231f, -0.1465125133f, 0.2302279044f, -0.3699435608f, 0.105700352f, -0.2646713633f, 0.3521828122f, -0.1864187807f, 0.1994492955f, 0.3937065066f, -0.3226158377f, 0.3796235338f, 0.1482921929f, -0.407400394f, 0.4212853031f,
	-0.2621297173f, -0.2536986953f, -0.2100236383f, 0.3624152444f, -0.3645038479f, 0.2318486784f, -0.3260457004f, -0.2130045332f, 0.3814998766f, -0.342977305f, -0.4355865605f, -0.2104679605f, 0.3348364681f, 0.3430468811f, -0.2291836801f, 0.2547707298f,
	0.4236174945f, -0.15387742f, -0.4407449312f, -0.06805276192f, 0.4453517192f, 0.2562464609f, 0.3278198355f, -0.4122774207f, 0.3354090914f, 0.446632869f, -0.1608953296f, -0.09463954939f, -0.02637688324f, 0.447102804f, -0.4365670908f, -0.3959858651f,
	-0.4240048207f, -0.3882794568f, -0.4283652566f, 0.3303888091f, 0.3321434919f, -0.413021046f, 0.08403060337f, -0.3822882919f, -0.3712395594f, 0.4472363971f, -0.4466591209f, 0.0486272539f, -0.4203101295f, 0.2205360833f, -0.3624900666f, -0.4036086833f,
	0.2152727807f, -0.4359392962f, 0.4178354266f, 0.2007630161f, -0.07278067175f, 0.3644748615f, -0.4317451775f, -0.297436456f, -0.2998672222f, -0.2673674124f, 0.2808423357f, 0.3498946567f, -0.2229685561f, 0.3305823267f, -0.2436681211f, -0.03402776529f,
	-0.319358823f, 0.4454633477f, 0.4483504221f, -0.4427358436f, 0.05452298565f, -0.2812560807f, 0.1266696921f, -0.3735981243f, 0.2959708351f, -0.3714377181f, -0.404467102f, 0.1636165687f, 0.3289185495f, -0.2494824991f, 0.03283133272f, -0.166306057f,
	-0.106833179f, 0.06440260376f, -0.4483230967f, -0.421377757f, 0.05097920662f, 0.2050584153f, 0.4178098529f, -0.3565189504f, 0.4478398129f, -0.3399999602f, 0.3767121994f, -0.3138934434f, -0.1462001792f, 0.3970290489f, 0.4459149305f, -0.4104889426f,
	0.1475103971f, 0.09258030352f, -0.1589664637f, 0.2482445008f, 0.4383624232f, 0.06242802956f, 0.2846591015f, -0.344202744f, 0.1198188883f, -0.243590703f, 0.2958191174f, -0.1164007991f, 0.1274037151f, 0.368047306f, 0.2451436949f, -0.4314509715f,
};
const float CELL_2D_Y[] =
{
	0.1281943404f, 0.415278375f, -0.3505218461f, 0.3517627718f, -0.3237467165f, -0.2967353402f, -0.09710417025f, -0.05953502905f, 0.3334085102f, -0.3965687458f, 0.3361990872f, 0.3871778202f, -0.04695150755f, 0.41340573f, -0.4171197882f, 0.4392750616f,
	0.4419948321f, -0.1857461384f, -0.2857157906f, -0.30038326f, -0.3868891648f, 0.449872789f, -0.05976119672f, 0.2631606867f, 0.2834856838f, 0.1803655873f, 0.3619887311f, -0.3142230135f, -0.3457315612f, -0.2258540565f, -0.02667811596f, 0.01430799601f,
	-0.4314657307f, 0.4485799926f, -0.1335025276f, 0.3145735065f, 0.3302087162f, 0.1751754899f, 0.2505256519f, -0.3904653228f, -0.4499902136f, -0.4146991995f, -0.08247647938f, -0.355112935f, -0.2023496216f, -0.2293938184f, 0.1326367019f, -0.4489736231f,
	0.2845441624f, -0.4476902368f, 0.4484280562f, 0.3095250049f, -0.4452222108f, 0.448706869f, 0.06742966669f, -0.1461850686f, 0.008627302568f, 0.4469356864f, -0.02055026661f, 0.4335725488f, -0.2924813028f, 0.2304191809f, -0.447738214f, 0.4455915232f,
	-0.2187385324f, -0.04988730975f, -0.2482076684f, 0.357223947f, 0.4166344988f, 0.1848760794f, -0.3130881435f, -0.3455761521f, 0.4005435199f, -0.08751256895f, 0.4172743077f, 0.4499824166f, 0.4003301853f, -0.4499587123f, -0.4338091548f, -0.2242337048f,
	0.4496698165f, 0.3340561458f, -0.4497451511f, -0.1757577897f, -0.4071547394f, 0.2488600778f, 0.3904147331f, -0.1700466149f, 0.267424695f, -0.4083336779f, 0.323561623f, 0.307891826f, 0.3870899429f, 0.2894847362f, -0.1698621719f, -0.3687717212f,
	0.2683874578f, 0.3886668486f, 0.3994821043f, -0.4421754202f, 0.1286329626f, 0.3105090899f, -0.4134275279f, 0.06566979625f, 0.2146355146f, 0.1436338239f, -0.2420141339f, -0.4476245948f, -0.2540826796f, -0.3698392535f, 0.4486549822f, 0.3294387544f,
	0.3893076172f, -0.1817925206f, -0.3434387019f, -0.348876097f, -0.3247973695f, -0.0566844308f, -0.3320331606f, 0.1899159123f, -0.2844501228f, 0.3096924441f, 0.3152548881f, 0.4498443737f, 0.1211526057f, 0.324405723f, 0.3579183483f, -0.1556373694f,
	-0.4385260051f, -0.3568750521f, 0.4254810025f, -0.3866459777f, 0.2562064828f, -0.4374099171f, 0.3639355292f, 0.2801200935f, -0.4095705534f, -0.4033856449f, 0.2179339044f, 0.3137180602f, 0.2416318948f, 0.4248640083f, 0.1911149365f, 0.1581729856f,
	0.3657704353f, -0.3716678248f, 0.3979825013f, 0.2667493029f, -0.2638881295f, 0.3856762766f, 0.3101519002f, -0.3963950918f, -0.2386584257f, 0.2913186713f, 0.1129794154f, 0.3977477059f, -0.3006402163f, 0.2912367377f, -0.3872658529f, -0.3709337882f,
	-0.151816397f, 0.4228731957f, 0.09079595574f, -0.444824484f, -0.06451237284f, -0.3699158705f, -0.3082761026f, -0.1803533432f, -0.3000012356f, -0.05494615882f, 0.4202531296f, 0.4399356268f, -0.4492262904f, -0.05098119915f, 0.1091291678f, 0.2137643437f,
	-0.1507312575f, 0.2274622243f, -0.1378521198f, 0.305521251f, -0.3036127481f, -0.1786438231f, -0.4420846725f, 0.2373934748f, -0.2543249683f, -0.04979563372f, 0.05473234629f, -0.4473649407f, -0.1607463688f, 0.39225481f, 0.2666476169f, -0.1989975647f,
	0.3951678503f, -0.1116106179f, 0.1670735057f, 0.4027334247f, -0.4440754146f, -0.2639281632f, 0.126870413f, 0.3376855855f, 0.3355289094f, 0.3619594822f, 0.3516071423f, 0.2829730186f, 0.390877248f, 0.3053118493f, -0.3783197679f, 0.4487116125f,
	0.3170330301f, -0.06373700535f, 0.03849544189f, -0.08052932871f, 0.4466847255f, 0.3512762688f, 0.4318041097f, 0.2508474468f, -0.3389708908f, 0.254035473f, -0.1972469604f, -0.419201167f, -0.3071035458f, -0.3745109914f, 0.4488007393f, -0.4181414777f,
	0.4371346153f, -0.4453676062f, 0.03881238203f, -0.1579265206f, -0.4471030312f, -0.4005634111f, -0.167137449f, -0.2745801121f, 0.04403977727f, -0.2947881053f, 0.2461461331f, 0.3224451987f, -0.4255884251f, -0.2118205239f, -0.06049689889f, -0.1843877112f,
	-0.4251360756f, 0.4403735771f, -0.4209865359f, 0.3753327428f, -0.1016778537f, 0.4456486745f, -0.3485243118f, -0.2898697484f, -0.4337550392f, 0.3783696201f, -0.3391033025f, 0.4346847754f, -0.4315881062f, 0.2589231171f, 0.3773652989f, 0.12786735f,
};
const float CELL_3D_X[] =
{
	0.1453787434f, -0.01242829687f, 0.2877979582f, -0.07732986802f, 0.1107205875f, 0.2755209141f, 0.294168941f, 0.4000921098f, -0.1697304074f, -0.1483224484f, 0.2623596946f, -0.2709003183f, -0.03516550699f, -0.1267712655f, 0.02952021915f, -0.2806854217f,
	-0.171159547f, 0.2113227183f, -0.1024352839f, -0.3304249877f, 0.2091111325f, 0.344678154f, 0.1984478035f, -0.2929008603f, -0.1617332831f, -0.3582060271f, -0.1852067326f, 0.3046301062f, -0.03816768434f, -0.4084952196f, -0.02687443361f, -0.03801098351f,
	0.2371120802f, 0.4447660503f, 0.01985147278f, 0.4274339143f, -0.2072988631f, -0.3791240978f, -0.2098721267f, 0.01582798878f, -0.1888129464f, 0.1612988974f, -0.08974491322f, 0.07041229526f, -0.1082925611f, 0.2474100658f, -0.1068836661f, 0.2396452163f,
	-0.3063886072f, 0.1593342891f, 0.2709690528f, -0.1519780427f, 0.1699773681f, -0.1986155616f, -0.1887482106f, 0.2659103394f, -0.08838976154f, -0.04201869311f, -0.3230334656f, 0.2612720941f, 0.385713046f, 0.07654967953f, 0.4317038818f, -0.2890436293f,
	-0.2201947582f, 0.4161322773f, 0.2204718095f, -0.1040307469f, -0.1432122615f, 0.3978380468f, -0.2599274663f, 0.4032618332f, -0.08953470255f, 0.118937202f, 0.02167047076f, -0.3411343612f, 0.3162964612f, 0.2355138889f, -0.02874541518f, -0.2461455173f,
	0.04208029445f, 0.2727458746f, -0.1347522818f, 0.3829624424f, -0.3547613644f, 0.2305790207f, -0.08323845599f, 0.2993663085f, -0.2154865723f, 0.01683355354f, 0.05240429123f, 0.00940104872f, 0.3465688735f, -0.3706867948f, 0.2741169781f, 0.06413433865f,
	-0.388187972f, 0.06419469312f, -0.1986120739f, -0.203203009f, -0.1389736354f, -0.06555641638f, -0.2529246486f, 0.1444476522f, -0.3643780054f, 0.4286142488f, 0.165872923f, 0.2219610524f, 0.04322940318f, -0.08481269795f, 0.1822082075f, -0.3269323334f,
	-0.4080485344f, 0.2676025294f, 0.3024892441f, 0.1448494052f, 0.4198402157f, -0.3008872161f, 0.3639310428f, 0.3295806598f, 0.2776259487f, 0.4149000507f, 0.145016715f, 0.09299023471f, 0.1028907093f, 0.2683057049f, -0.4227307273f, -0.1781224702f,
	0.4390788626f, 0.2972583585f, -0.1707002821f, 0.3806686614f, -0.1751445661f, -0.2227237566f, 0.1369633021f, -0.3529503428f, -0.2590744185f, -0.3784019401f, -0.05635805671f, 0.3251428613f, -0.4190995804f, -0.3253150961f, 0.2857945863f, -0.2733604046f,
	0.219003657f, 0.3182767252f, -0.03222023115f, -0.3087780231f, -0.06487611647f, 0.3921171432f, -0.1606404506f, -0.03767771199f, 0.1394866832f, -0.4345093872f, -0.1044637494f, 0.2658727501f, 0.2051461999f, -0.266085566f, 0.07849405464f, -0.2160686338f,
	-0.185779186f, 0.02492421743f, -0.120167831f, -0.02160084693f, 0.2597670064f, -0.1611553854f, -0.3278896792f, 0.2822734956f, 0.03169341113f, 0.2202613604f, 0.2933396046f, -0.3194922995f, -0.3441586045f, 0.2703645948f, 0.2298568861f, 0.09326603877f,
	-0.1116165319f, 0.2172907365f, 0.1991339479f, -0.0541918155f, 0.08871336998f, 0.2787673278f, -0.322166438f, -0.4277366384f, 0.240131882f, 0.1448607981f, -0.3837065682f, -0.4382627882f, -0.37728353f, 0.1259579313f, -0.1406285511f, -0.1580694418f,
	0.2477612106f, 0.2916132853f, 0.07365265219f, -0.26126526f, -0.3721862032f, -0.3691191571f, 0.2278441737f, 0.363398169f, -0.304231482f, -0.3199312232f, 0.2874852279f, -0.1451096801f, 0.3220090754f, -0.1247400865f, -0.2829555867f, 0.1069384374f,
	-0.1420661144f, -0.250548338f, 0.3265787872f, 0.07646097258f, 0.3451771584f, 0.298137964f, 0.2812250376f, 0.4390345476f, 0.2148373234f, 0.2595421179f, 0.3182823114f, -0.4089859285f, -0.2826749061f, 0.3483864637f, -0.3226415069f, 0.4330734858f,
	-0.08717822568f, -0.2149678299f, -0.2687330705f, 0.2105665099f, 0.4361845915f, 0.05333333359f, -0.05986216652f, 0.3664988455f, -0.2341015558f, -0.04730947785f, -0.2391566239f, -0.1242081035f, 0.2614832715f, -0.2728794681f, 0.007892900508f, -0.01730330376f,
	0.2054835762f, -0.3231994983f, -0.2669545963f, -0.05554372779f, -0.2083935713f, 0.06989323478f, 0.3847566193f, -0.3026215288f, 0.3450735512f, 0.1814473292f, -0.03855010448f, 0.3533670318f, -0.007945601311f, 0.4063099273f, -0.2016773589f, -0.07527055435f,
};
const float CELL_3D_Y[] =
{
	-0.4149781685f, -0.1457918398f, -0.02606483451f, 0.2377094325f, -0.3552302079f, 0.2640521179f, 0.1526064594f, -0.2034056362f, 0.3970864695f, -0.3859694688f, -0.2354852944f, 0.3505271138f, 0.3885234328f, 0.1920044036f, 0.4409685861f, -0.266996757f,
	0.2141185563f, 0.3902405947f, 0.2128044156f, -0.1566986703f, 0.3133278055f, -0.1944240454f, -0.3214342325f, 0.2262915116f, 0.006314769776f, -0.148303178f, -0.3454119342f, 0.1026310383f, -0.2551766358f, 0.1805950793f, -0.2749741471f, 0.3277859044f,
	0.2900386767f, 0.03946930643f, -0.01503183293f, 0.03345994256f, 0.2871414597f, 0.1281177671f, -0.1007087278f, 0.4263894424f, -0.3160996813f, -0.1974805082f, 0.229148752f, 0.4150230285f, -0.1586061639f, -0.3309414609f, -0.2701644537f, 0.06803600538f,
	0.2597428179f, -0.3114350249f, 0.1412648683f, 0.3623355133f, 0.3456012883f, 0.3836276443f, -0.2050154888f, 0.3015631259f, -0.4288819642f, 0.3099592485f, 0.201549922f, 0.2759854499f, 0.2193460345f, 0.3721732183f, -0.02577753072f, -0.3418179959f,
	0.383023377f, -0.1669634289f, 0.02654238946f, 0.3890079625f, 0.371614387f, -0.06206669342f, 0.2616724959f, -0.1124593585f, -0.3048244735f, -0.2875221847f, -0.03284630549f, 0.2500031105f, 0.3082064153f, -0.3439334267f, -0.3955933019f, 0.02020282325f,
	-0.4470439576f, 0.2288471896f, -0.02720848277f, 0.1231931484f, 0.1271702173f, 0.3063895591f, -0.1922245118f, -0.2619918095f, 0.2706747713f, -0.2680655787f, 0.4335128183f, -0.4472890582f, 0.01141914583f, -0.2551104378f, 0.2139972417f, 0.1708718512f,
	-0.03973280434f, -0.2803682491f, -0.3391173584f, -0.3871641506f, -0.2775901578f, 0.342253257f, -0.2904227915f, 0.1069184044f, -0.2447099973f, -0.1358496089f, -0.3136808464f, -0.3658139958f, -0.3832730794f, -0.4404869674f, -0.3953259299f, 0.3036542563f,
	0.04227858267f, -0.01299671652f, -0.1009990293f, 0.425921681f, 0.08062320474f, -0.333040905f, -0.1291284382f, 0.0184175994f, -0.2974929052f, -0.144793182f, -0.0398992945f, -0.299732164f, -0.361266869f, -0.07076041213f, -0.07933161816f, 0.1806857196f,
	-0.02841848598f, 0.2382799621f, 0.2215845691f, 0.1471852559f, -0.274887877f, -0.2316778837f, 0.1341343041f, -0.2472893463f, -0.2985577559f, 0.2199816631f, 0.1485737441f, 0.09666046873f, 0.1406751354f, -0.3080335042f, -0.05796152095f, 0.1973770973f,
	0.2410037886f, -0.271342949f, -0.3331161506f, 0.1992794134f, -0.4311322747f, -0.06294284106f, -0.358928121f, -0.2290351443f, -0.3602213994f, 0.005751117145f, 0.4168128432f, 0.2551943237f, 0.1975390727f, 0.23483312f, -0.3300346342f, 0.05376451292f,
	0.2148499206f, -0.3229954284f, 0.4017266681f, -0.06885389554f, 0.3096300784f, -0.09823036005f, 0.1461670309f, 0.03754421121f, 0.347405252f, -0.3460788041f, 0.3031973659f, 0.2453752201f, -0.1698856132f, -0.3574277231f, 0.3744156221f, -0.3170108894f,
	-0.2985018719f, -0.3460005203f, 0.3820341668f, -0.2103145071f, 0.2012117383f, 0.3505404674f, 0.3067213525f, 0.132066775f, -0.1612516055f, -0.2387819045f, -0.2206398454f, -0.09082753406f, 0.05445141085f, 0.348394558f, -0.270877371f, 0.4162931958f,
	-0.2927867412f, 0.3312535401f, -0.1666159848f, -0.2422237692f, 0.252790166f, -0.255281188f, -0.3358364886f, -0.2310190248f, -0.2698452035f, 0.316332536f, 0.1642275508f, 0.3277541114f, 0.0511344108f, -0.04333605335f, -0.3056190617f, 0.3491024667f,
	-0.3055376754f, 0.3156466809f, 0.1871229129f, -0.3026690852f, 0.2757120714f, 0.2852657134f, 0.3466716415f, -0.09790429955f, 0.1850172527f, -0.07946825393f, -0.307355516f, -0.04647718411f, 0.07417482322f, 0.225442246f, -0.1420585388f, -0.118868561f,
	-0.3909896417f, 0.3939973956f, 0.322686276f, -0.1961317136f, -0.1105517485f, -0.313639498f, 0.1361029153f, 0.2550543014f, -0.182405731f, -0.4222150243f, -0.2577696514f, 0.4256953395f, -0.3650179274f, -0.3499628774f, -0.1672771315f, 0.2978486637f,
	-0.3252600376f, 0.1564282844f, 0.2599343665f, 0.3170813944f, -0.310922837f, -0.3156141536f, -0.1605309138f, -0.3001537679f, 0.08611519592f, -0.2788782453f, 0.09795110726f, 0.2665752752f, 0.140359426f, -0.1491768253f, 0.008816271194f, -0.425643481f,
};
const float CELL_3D_Z[] =
{
	-0.0956981749f, -0.4255470325f, -0.3449535616f, 0.3741848704f, -0.2530858567f, -0.238463215f, 0.3044271714f, 0.03244149937f, -0.1265461359f, 0.1775613147f, 0.2796677792f, -0.07901746678f, 0.2243054374f, 0.3867342179f, 0.08470692262f, 0.2289725438f,
	0.3568720405f, -0.07453178509f, -0.3830421561f, 0.2622305365f, -0.2461670583f, -0.2142341261f, -0.2445373252f, 0.2559320961f, -0.4198838754f, -0.2284613961f, -0.2211087107f, 0.314908508f, -0.3686842991f, 0.05492788837f, 0.3551999201f, 0.3059600725f,
	-0.2493099024f, 0.05590469027f, -0.4493105419f, -0.1366772882f, -0.2776273824f, 0.2057929936f, -0.3851122467f, 0.1429738373f, -0.2587096108f, -0.3707885038f, -0.3767448739f, -0.1590534329f, 0.4069604477f, 0.1782302128f, -0.3436379634f, -0.3747549496f,
	0.2028785103f, -0.2830561951f, -0.3303331794f, 0.2193527988f, 0.2327390037f, -0.1260225743f, -0.353330953f, -0.2021172246f, -0.1036702021f, 0.3235115047f, -0.2398478873f, -0.2409749453f, 0.07491837764f, 0.241095919f, 0.1243675091f, -0.04598084447f,
	-0.08548310451f, -0.03817251927f, -0.391391981f, -0.2008741118f, -0.2095065525f, 0.2009293758f, -0.2578084893f, 0.1650235939f, 0.3186935478f, 0.325092195f, -0.4482761547f, 0.1537068389f, -0.08640228117f, -0.1695376245f, 0.2125550295f, -0.3761704803f,
	0.02968078139f, -0.2752065618f, -0.4284874806f, -0.2016512234f, 0.2459107769f, 0.2354968222f, 0.3982726409f, -0.2103333191f, 0.287751117f, -0.3610505186f, -0.1087217856f, 0.04841609928f, -0.2868093776f, 0.003156692623f, -0.2855959784f, 0.4113266307f,
	-0.2241236325f, 0.3460819069f, 0.2192091725f, 0.1063600375f, -0.3257760473f, -0.2847192729f, 0.2327739768f, 0.4125570634f, -0.09922543227f, -0.01829506817f, -0.2767498872f, 0.1393320198f, 0.2318037215f, -0.03574965489f, 0.1140946023f, 0.05838957105f,
	-0.184956522f, 0.36155217f, -0.3174892964f, -0.0104580805f, 0.1404780841f, -0.03241355801f, -0.2310412139f, -0.3058388149f, -0.1921504723f, -0.09691688386f, 0.4241205002f, -0.3225111565f, 0.247789732f, -0.3542668666f, -0.1323073187f, -0.3716517945f,
	-0.09435116353f, -0.2394997452f, 0.3525077196f, -0.1895464869f, 0.3102596268f, 0.3149912482f, -0.4071228836f, -0.129514612f, -0.2150435121f, -0.1044989934f, 0.4210102279f, -0.2957006485f, -0.08405978803f, -0.04225456877f, 0.3427271751f, -0.2980207554f,
	-0.3105713639f, 0.1660509868f, -0.300824678f, -0.2596995338f, 0.1114273361f, -0.2116183942f, -0.2187812825f, 0.3855169162f, 0.2308332918f, 0.1169124335f, -0.1336202785f, 0.2582393035f, 0.3484154868f, 0.2766800993f, -0.2956616708f, -0.3910546287f,
	0.3490352499f, -0.3123343347f, 0.1633259825f, 0.4441762538f, 0.1978643903f, 0.4085091653f, 0.2713366126f, -0.3484423997f, -0.2842624114f, -0.1849713341f, 0.1565989581f, -0.200538455f, -0.2349334659f, 0.04060059933f, 0.0973588921f, 0.3054595587f,
	0.3177080142f, -0.1885958001f, -0.1299829458f, 0.39412061f, 0.3926114802f, 0.04370535101f, 0.06804996813f, 0.04582286686f, 0.344723946f, 0.3528435224f, 0.08116235683f, -0.04664855374f, 0.2391488697f, 0.2554522098f, -0.3306796947f, -0.06491553533f,
	-0.2353514536f, 0.08793624968f, 0.411478311f, 0.2748965434f, 0.008634938242f, 0.03290232422f, 0.1944244981f, 0.1306597909f, 0.1926830856f, -0.008816977938f, -0.304764754f, -0.2720669462f, 0.3101538769f, -0.4301882115f, -0.1703910946f, -0.2630430352f,
	-0.2982682484f, -0.2002316239f, 0.2466400438f, 0.324106687f, -0.0856480183f, 0.179547284f, 0.05684409612f, -0.01278335452f, 0.3494474791f, 0.3589187731f, -0.08203022006f, 0.1818526372f, 0.3421885344f, -0.1740766085f, -0.2796816575f, -0.02859407492f,
	-0.2050050172f, -0.03247898316f, -0.1617284888f, -0.3459683451f, 0.004616608544f, -0.3182543336f, -0.4247264031f, -0.05590974511f, 0.3382670703f, -0.1483114513f, -0.2808182972f, -0.07652336246f, 0.02980623099f, 0.07458404908f, 0.4176793787f, -0.3368779738f,
	-0.2334146693f, -0.2712420987f, -0.2523278991f, -0.3144428146f, -0.2497981362f, 0.3130537363f, -0.1693876312f, -0.1443188342f, 0.2756962409f, -0.3029914042f, 0.4375151083f, 0.08105160988f, -0.4274764309f, -0.1231199324f, -0.4021797064f, -0.1251477955f,
};

static int FastFloor(float f) { return (f >= 0.0f ? (int)f : (int)f - 1); }
static int FastRound(float f) { return (f >= 0.0f) ? (int)(f + 0.5f) : (int)(f - 0.5f); }
static float FastAbs(float f) { return fabsf(f); }
static int FastAbs(int i) { return abs(i); }
static float Lerp(float a, float b, float t) { return a + t * (b - a); }
static float InterpHermiteFunc(float t) { return t*t*(3 - 2 * t); }
static float InterpQuinticFunc(float t) { return t*t*t*(t*(t * 6 - 15) + 10); }

void FastNoise::SetSeed(int seed)
{
	m_seed = seed;

	std::mt19937 gen(seed);

	for (int i = 0; i < 256; i++)
		m_perm[i] = i;

	for (int j = 0; j < 256; j++)
	{
		std::uniform_int_distribution<> dis(0, 256 - j);
		int k = dis(gen) + j;
		int l = m_perm[j];
		m_perm[j] = m_perm[j + 256] = m_perm[k];
		m_perm[k] = l;
		m_perm12[j] = m_perm12[j + 256] = m_perm[j] % 12;
	}
}

unsigned char FastNoise::Index2D_12(unsigned char offset, int x, int y)
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
unsigned char FastNoise::Index3D_12(unsigned char offset, int x, int y, int z)
{
	return m_perm12[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}
unsigned char FastNoise::Index4D_32(unsigned char offset, int x, int y, int z, int w)
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]] & 31;
}
unsigned char FastNoise::Index2D_256(unsigned char offset, int x, int y)
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + offset]];
}
unsigned char FastNoise::Index3D_256(unsigned char offset, int x, int y, int z)
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + offset]]];
}
unsigned char FastNoise::Index4D_256(unsigned char offset, int x, int y, int z, int w)
{
	return m_perm[(x & 0xff) + m_perm[(y & 0xff) + m_perm[(z & 0xff) + m_perm[(w & 0xff) + offset]]]];
}

// Hashing
#define X_PRIME 1619
#define Y_PRIME 31337
#define Z_PRIME 6971
#define W_PRIME 1013

static float ValCoord2D(int seed, int x, int y)
{
	int n = X_PRIME * x;
	n += Y_PRIME * y;
	n += seed;
	n &= 0x7fffffff;
	n = (n >> 13) ^ n;
	return 9.311924889611565e-10f * (((n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff) - 1073891824);
}
static float ValCoord3D(int seed, int x, int y, int z)
{
	int n = X_PRIME * x;
	n += Y_PRIME * y;
	n += Z_PRIME * z;
	n += seed;
	n &= 0x7fffffff;
	n = (n >> 13) ^ n;
	return 9.311924889611565e-10f * (((n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff) - 1073891824);
}
static float ValCoord4D(int seed, int x, int y, int z, int w)
{
	int n = X_PRIME * x;
	n += Y_PRIME * y;
	n += Z_PRIME * z;
	n += W_PRIME * w;
	n += seed;
	n &= 0x7fffffff;
	n = (n >> 13) ^ n;
	return 9.311924889611565e-10f * (((n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff) - 1073891824);
}

float FastNoise::ValCoord2DFast(unsigned char offset, int x, int y)
{
	return VAL_LUT[Index2D_256(offset, x, y)];
}
float FastNoise::ValCoord3DFast(unsigned char offset, int x, int y, int z)
{
	return VAL_LUT[Index3D_256(offset, x, y, z)];
}

float FastNoise::GradCoord2D(unsigned char offset, int x, int y, float xd, float yd)
{
	unsigned char lutPos = Index2D_12(offset, x, y);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos];
}
float FastNoise::GradCoord3D(unsigned char offset, int x, int y, int z, float xd, float yd, float zd)
{
	unsigned char lutPos = Index3D_12(offset, x, y, z);

	return xd*GRAD_X[lutPos] + yd*GRAD_Y[lutPos] + zd*GRAD_Z[lutPos];
}
float FastNoise::GradCoord4D(unsigned char offset, int x, int y, int z, int w, float xd, float yd, float zd, float wd)
{
	unsigned char lutPos = Index4D_32(offset, x, y, z, w) << 2;

	return xd*GRAD_4D[lutPos] + yd*GRAD_4D[lutPos + 1] + zd*GRAD_4D[lutPos + 2] + wd*GRAD_4D[lutPos + 3];
}

float FastNoise::GetNoise(float x, float y, float z)
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_noiseType)
	{
	case Value:
		return SingleValue(0, x, y, z);
	case ValueFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleValueFractalFBM(x, y, z);
		case Billow:
			return SingleValueFractalBillow(x, y, z);
		case RigidMulti:
			return SingleValueFractalRigidMulti(x, y, z);
		default:
			return 0.0f;
		}
	case Gradient:
		return SingleGradient(0, x, y, z);
	case GradientFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleGradientFractalFBM(x, y, z);
		case Billow:
			return SingleGradientFractalBillow(x, y, z);
		case RigidMulti:
			return SingleGradientFractalRigidMulti(x, y, z);
		default:
			return 0.0f;
		}
	case Simplex:
		return SingleSimplex(0, x, y, z);
	case SimplexFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleSimplexFractalFBM(x, y, z);
		case Billow:
			return SingleSimplexFractalBillow(x, y, z);
		case RigidMulti:
			return SingleSimplexFractalRigidMulti(x, y, z);
		default:
			return 0.0f;
		}
	case Cellular:
		switch (m_cellularReturnType)
		{
		case CellValue:
		case NoiseLookup:
		case Distance:
			return SingleCellular(x, y, z);
		default:
			return SingleCellular2Edge(x, y, z);
		}
	case WhiteNoise:
		return GetWhiteNoise(x, y, z);
	default:
		return 0.0f;
	}
}

float FastNoise::GetNoise(float x, float y)
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_noiseType)
	{
	case Value:
		return SingleValue(0, x, y);
	case ValueFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleValueFractalFBM(x, y);
		case Billow:
			return SingleValueFractalBillow(x, y);
		case RigidMulti:
			return SingleValueFractalRigidMulti(x, y);
		default:
			return 0.0f;
		}
	case Gradient:
		return SingleGradient(0, x, y);
	case GradientFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleGradientFractalFBM(x, y);
		case Billow:
			return SingleGradientFractalBillow(x, y);
		case RigidMulti:
			return SingleGradientFractalRigidMulti(x, y);
		default:
			return 0.0f;
		}
	case Simplex:
		return SingleSimplex(0, x, y);
	case SimplexFractal:
		switch (m_fractalType)
		{
		case FBM:
			return SingleSimplexFractalFBM(x, y);
		case Billow:
			return SingleSimplexFractalBillow(x, y);
		case RigidMulti:
			return SingleSimplexFractalRigidMulti(x, y);
		default:
			return 0.0f;
		}
	case Cellular:
		switch (m_cellularReturnType)
		{
		case CellValue:
		case NoiseLookup:
		case Distance:
			return SingleCellular(x, y);
		default:
			return SingleCellular2Edge(x, y);
		}
	case WhiteNoise:
		return GetWhiteNoise(x, y);
	default:
		return 0.0f;
	}
}

// White Noise
float FastNoise::GetWhiteNoise(float x, float y, float z, float w)
{
	return ValCoord4D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16),
		*reinterpret_cast<int*>(&z) ^ (*reinterpret_cast<int*>(&z) >> 16),
		*reinterpret_cast<int*>(&w) ^ (*reinterpret_cast<int*>(&w) >> 16));
}

float FastNoise::GetWhiteNoise(float x, float y, float z)
{
	return ValCoord3D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16),
		*reinterpret_cast<int*>(&z) ^ (*reinterpret_cast<int*>(&z) >> 16));
}

float FastNoise::GetWhiteNoise(float x, float y)
{
	return ValCoord2D(m_seed,
		*reinterpret_cast<int*>(&x) ^ (*reinterpret_cast<int*>(&x) >> 16),
		*reinterpret_cast<int*>(&y) ^ (*reinterpret_cast<int*>(&y) >> 16));
}

float FastNoise::GetWhiteNoiseInt(int x, int y, int z, int w)
{
	return ValCoord4D(m_seed, x, y, z, w);
}

float FastNoise::GetWhiteNoiseInt(int x, int y, int z)
{
	return ValCoord3D(m_seed, x, y, z);
}

float FastNoise::GetWhiteNoiseInt(int x, int y)
{
	return ValCoord2D(m_seed, x, y);
}

// Value Noise
float FastNoise::GetValueFractal(float x, float y, float z)
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalFBM(x, y, z);
	case Billow:
		return SingleValueFractalBillow(x, y, z);
	case RigidMulti:
		return SingleValueFractalRigidMulti(x, y, z);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleValueFractalFBM(float x, float y, float z)
{
	float sum = SingleValue(m_perm[0], x, y, z);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleValue(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleValueFractalBillow(float x, float y, float z)
{
	float sum = FastAbs(SingleValue(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleValue(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleValueFractalRigidMulti(float x, float y, float z)
{
	float sum = 1.0f - FastAbs(SingleValue(m_perm[0], x, y, z));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleValue(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

float FastNoise::GetValue(float x, float y, float z)
{
	return SingleValue(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float FastNoise::SingleValue(unsigned char offset, float x, float y, float z)
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	float xs, ys, zs;
	switch (m_interp)
	{
	case Linear:
		xs = x - (float)x0;
		ys = y - (float)y0;
		zs = z - (float)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (float)x0);
		ys = InterpHermiteFunc(y - (float)y0);
		zs = InterpHermiteFunc(z - (float)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (float)x0);
		ys = InterpQuinticFunc(y - (float)y0);
		zs = InterpQuinticFunc(z - (float)z0);
		break;
	}

	float xf00 = Lerp(ValCoord3DFast(offset, x0, y0, z0), ValCoord3DFast(offset, x1, y0, z0), xs);
	float xf10 = Lerp(ValCoord3DFast(offset, x0, y1, z0), ValCoord3DFast(offset, x1, y1, z0), xs);
	float xf01 = Lerp(ValCoord3DFast(offset, x0, y0, z1), ValCoord3DFast(offset, x1, y0, z1), xs);
	float xf11 = Lerp(ValCoord3DFast(offset, x0, y1, z1), ValCoord3DFast(offset, x1, y1, z1), xs);

	float yf0 = Lerp(xf00, xf10, ys);
	float yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

float FastNoise::GetValueFractal(float x, float y)
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleValueFractalFBM(x, y);
	case Billow:
		return SingleValueFractalBillow(x, y);
	case RigidMulti:
		return SingleValueFractalRigidMulti(x, y);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleValueFractalFBM(float x, float y)
{
	float sum = SingleValue(m_perm[0], x, y);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleValue(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleValueFractalBillow(float x, float y)
{
	float sum = FastAbs(SingleValue(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		amp *= m_gain;
		sum += (FastAbs(SingleValue(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleValueFractalRigidMulti(float x, float y)
{
	float sum = 1.0f - FastAbs(SingleValue(m_perm[0], x, y));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleValue(m_perm[i], x, y))) * amp;
	}

	return sum;
}

float FastNoise::GetValue(float x, float y)
{
	return SingleValue(0, x * m_frequency, y * m_frequency);
}

float FastNoise::SingleValue(unsigned char offset, float x, float y)
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float xs, ys;
	switch (m_interp)
	{
	case Linear:
		xs = x - (float)x0;
		ys = y - (float)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (float)x0);
		ys = InterpHermiteFunc(y - (float)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (float)x0);
		ys = InterpQuinticFunc(y - (float)y0);
		break;
	}

	float xf0 = Lerp(ValCoord2DFast(offset, x0, y0), ValCoord2DFast(offset, x1, y0), xs);
	float xf1 = Lerp(ValCoord2DFast(offset, x0, y1), ValCoord2DFast(offset, x1, y1), xs);

	return Lerp(xf0, xf1, ys);
}

// Gradient Noise
float FastNoise::GetGradientFractal(float x, float y, float z)
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleGradientFractalFBM(x, y, z);
	case Billow:
		return SingleGradientFractalBillow(x, y, z);
	case RigidMulti:
		return SingleGradientFractalRigidMulti(x, y, z);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleGradientFractalFBM(float x, float y, float z)
{
	float sum = SingleGradient(m_perm[0], x, y, z);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleGradient(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleGradientFractalBillow(float x, float y, float z)
{
	float sum = FastAbs(SingleGradient(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleGradient(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleGradientFractalRigidMulti(float x, float y, float z)
{
	float sum = 1.0f - FastAbs(SingleGradient(m_perm[0], x, y, z));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleGradient(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

float FastNoise::GetGradient(float x, float y, float z)
{
	return SingleGradient(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

float FastNoise::SingleGradient(unsigned char offset, float x, float y, float z)
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int z0 = FastFloor(z);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	float xs, ys, zs;
	switch (m_interp)
	{
	case Linear:
		xs = x - (float)x0;
		ys = y - (float)y0;
		zs = z - (float)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (float)x0);
		ys = InterpHermiteFunc(y - (float)y0);
		zs = InterpHermiteFunc(z - (float)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (float)x0);
		ys = InterpQuinticFunc(y - (float)y0);
		zs = InterpQuinticFunc(z - (float)z0);
		break;
	}

	float xd0 = x - (float)x0;
	float yd0 = y - (float)y0;
	float zd0 = z - (float)z0;
	float xd1 = xd0 - 1.0f;
	float yd1 = yd0 - 1.0f;
	float zd1 = zd0 - 1.0f;

	float xf00 = Lerp(GradCoord3D(offset, x0, y0, z0, xd0, yd0, zd0), GradCoord3D(offset, x1, y0, z0, xd1, yd0, zd0), xs);
	float xf10 = Lerp(GradCoord3D(offset, x0, y1, z0, xd0, yd1, zd0), GradCoord3D(offset, x1, y1, z0, xd1, yd1, zd0), xs);
	float xf01 = Lerp(GradCoord3D(offset, x0, y0, z1, xd0, yd0, zd1), GradCoord3D(offset, x1, y0, z1, xd1, yd0, zd1), xs);
	float xf11 = Lerp(GradCoord3D(offset, x0, y1, z1, xd0, yd1, zd1), GradCoord3D(offset, x1, y1, z1, xd1, yd1, zd1), xs);

	float yf0 = Lerp(xf00, xf10, ys);
	float yf1 = Lerp(xf01, xf11, ys);

	return Lerp(yf0, yf1, zs);
}

float FastNoise::GetGradientFractal(float x, float y)
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleGradientFractalFBM(x, y);
	case Billow:
		return SingleGradientFractalBillow(x, y);
	case RigidMulti:
		return SingleGradientFractalRigidMulti(x, y);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleGradientFractalFBM(float x, float y)
{
	float sum = SingleGradient(m_perm[0], x, y);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleGradient(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleGradientFractalBillow(float x, float y)
{
	float sum = FastAbs(SingleGradient(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleGradient(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleGradientFractalRigidMulti(float x, float y)
{
	float sum = 1.0f - FastAbs(SingleGradient(m_perm[0], x, y));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleGradient(m_perm[i], x, y))) * amp;
	}

	return sum;
}

float FastNoise::GetGradient(float x, float y)
{
	return SingleGradient(0, x * m_frequency, y * m_frequency);
}

float FastNoise::SingleGradient(unsigned char offset, float x, float y)
{
	int x0 = FastFloor(x);
	int y0 = FastFloor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float xs, ys;
	switch (m_interp)
	{
	case Linear:
		xs = x - (float)x0;
		ys = y - (float)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(x - (float)x0);
		ys = InterpHermiteFunc(y - (float)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(x - (float)x0);
		ys = InterpQuinticFunc(y - (float)y0);
		break;
	}

	float xd0 = x - (float)x0;
	float yd0 = y - (float)y0;
	float xd1 = xd0 - 1.0f;
	float yd1 = yd0 - 1.0f;

	float xf0 = Lerp(GradCoord2D(offset, x0, y0, xd0, yd0), GradCoord2D(offset, x1, y0, xd1, yd0), xs);
	float xf1 = Lerp(GradCoord2D(offset, x0, y1, xd0, yd1), GradCoord2D(offset, x1, y1, xd1, yd1), xs);

	return Lerp(xf0, xf1, ys);
}

// Simplex Noise

float FastNoise::GetSimplexFractal(float x, float y, float z)
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM(x, y, z);
	case Billow:
		return SingleSimplexFractalBillow(x, y, z);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti(x, y, z);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleSimplexFractalFBM(float x, float y, float z)
{
	float sum = SingleSimplex(m_perm[0], x, y, z);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex(m_perm[i], x, y, z) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleSimplexFractalBillow(float x, float y, float z)
{
	float sum = FastAbs(SingleSimplex(m_perm[0], x, y, z)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex(m_perm[i], x, y, z)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleSimplexFractalRigidMulti(float x, float y, float z)
{
	float sum = 1.0f - FastAbs(SingleSimplex(m_perm[0], x, y, z));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;
		z *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleSimplex(m_perm[i], x, y, z))) * amp;
	}

	return sum;
}

float FastNoise::GetSimplex(float x, float y, float z)
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency);
}

static const float F3 = 1.0f / 3.0f;
static const float G3 = 1.0f / 6.0f;

float FastNoise::SingleSimplex(unsigned char offset, float x, float y, float z)
{
	float t = (x + y + z) * F3;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);
	int k = FastFloor(z + t);

	t = (i + j + k) * G3;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;

	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;

	if (x0 >= y0)
	{
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
		else if (x0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
		}
		else // x0 < z0
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
		}
	}
	else // x0 < y0
	{
		if (y0 < z0)
		{
			i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
		}
		else if (x0 < z0)
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
		}
		else // x0 >= z0
		{
			i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		}
	}

	float x1 = x0 - i1 + G3;
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0f*G3;
	float y2 = y0 - j2 + 2.0f*G3;
	float z2 = z0 - k2 + 2.0f*G3;
	float x3 = x0 - 1.0f + 3.0f*G3;
	float y3 = y0 - 1.0f + 3.0f*G3;
	float z3 = z0 - 1.0f + 3.0f*G3;

	float n0, n1, n2, n3;

	t = 0.6f - x0*x0 - y0*y0 - z0*z0;
	if (t < 0.0f) n0 = 0.0f;
	else
	{
		t *= t;
		n0 = t*t*GradCoord3D(offset, i, j, k, x0, y0, z0);
	}

	t = 0.6f - x1*x1 - y1*y1 - z1*z1;
	if (t < 0.0f) n1 = 0.0f;
	else
	{
		t *= t;
		n1 = t*t*GradCoord3D(offset, i + i1, j + j1, k + k1, x1, y1, z1);
	}

	t = 0.6f - x2*x2 - y2*y2 - z2*z2;
	if (t < 0.0f) n2 = 0.0f;
	else
	{
		t *= t;
		n2 = t*t*GradCoord3D(offset, i + i2, j + j2, k + k2, x2, y2, z2);
	}

	t = 0.6f - x3*x3 - y3*y3 - z3*z3;
	if (t < 0.0f) n3 = 0.0f;
	else
	{
		t *= t;
		n3 = t*t*GradCoord3D(offset, i + 1, j + 1, k + 1, x3, y3, z3);
	}

	return 32.0f * (n0 + n1 + n2 + n3);
}

float FastNoise::GetSimplexFractal(float x, float y)
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_fractalType)
	{
	case FBM:
		return SingleSimplexFractalFBM(x, y);
	case Billow:
		return SingleSimplexFractalBillow(x, y);
	case RigidMulti:
		return SingleSimplexFractalRigidMulti(x, y);
	default:
		return 0.0f;
	}
}

float FastNoise::SingleSimplexFractalFBM(float x, float y)
{
	float sum = SingleSimplex(m_perm[0], x, y);
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += SingleSimplex(m_perm[i], x, y) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleSimplexFractalBillow(float x, float y)
{
	float sum = FastAbs(SingleSimplex(m_perm[0], x, y)) * 2.0f - 1.0f;
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum += (FastAbs(SingleSimplex(m_perm[i], x, y)) * 2.0f - 1.0f) * amp;
	}

	return sum * m_fractalBounding;
}

float FastNoise::SingleSimplexFractalRigidMulti(float x, float y)
{
	float sum = 1.0f - FastAbs(SingleSimplex(m_perm[0], x, y));
	float amp = 1.0f;
	unsigned int i = 0;

	while (++i < m_octaves)
	{
		x *= m_lacunarity;
		y *= m_lacunarity;

		amp *= m_gain;
		sum -= (1.0f - FastAbs(SingleSimplex(m_perm[i], x, y))) * amp;
	}

	return sum;
}

float FastNoise::GetSimplex(float x, float y)
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency);
}

static const float F2 = 1.f / 2.f;
static const float G2 = 1.f / 4.f;

float FastNoise::SingleSimplex(unsigned char offset, float x, float y)
{
	float t = (x + y) * F2;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);

	t = (i + j) * G2;
	float X0 = i - t;
	float Y0 = j - t;

	float x0 = x - X0;
	float y0 = y - Y0;

	int i1, j1;
	if (x0 > y0)
	{
		i1 = 1; j1 = 0;
	}
	else
	{
		i1 = 0; j1 = 1;
	}

	float x1 = x0 - (float)i1 + G2;
	float y1 = y0 - (float)j1 + G2;
	float x2 = x0 - 1.0f + 2.0f*G2;
	float y2 = y0 - 1.0f + 2.0f*G2;

	float n0, n1, n2;

	t = 0.5f - x0*x0 - y0*y0;
	if (t < 0) n0 = 0;
	else
	{
		t *= t;
		n0 = t * t * GradCoord2D(offset, i, j, x0, y0);
	}

	t = 0.5f - x1*x1 - y1*y1;
	if (t < 0) n1 = 0;
	else
	{
		t *= t;
		n1 = t*t*GradCoord2D(offset, i + i1, j + j1, x1, y1);
	}

	t = 0.5f - x2*x2 - y2*y2;
	if (t < 0) n2 = 0;
	else
	{
		t *= t;
		n2 = t*t*GradCoord2D(offset, i + 1, j + 1, x2, y2);
	}

	return  50.0f * (n0 + n1 + n2);
}

float FastNoise::GetSimplex(float x, float y, float z, float w)
{
	return SingleSimplex(0, x * m_frequency, y * m_frequency, z * m_frequency, w * m_frequency);
}

static const int SIMPLEX_4D[] =
{
	0,1,2,3,0,1,3,2,0,0,0,0,0,2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,0,
	0,2,1,3,0,0,0,0,0,3,1,2,0,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,3,2,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,2,0,3,0,0,0,0,1,3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,3,0,1,2,3,1,0,
	1,0,2,3,1,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,3,1,0,0,0,0,2,1,3,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	2,0,1,3,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,2,3,0,2,1,0,0,0,0,3,1,2,0,
	2,1,0,3,0,0,0,0,0,0,0,0,0,0,0,0,3,1,0,2,0,0,0,0,3,2,0,1,3,2,1,0
};

static const float F4 = (sqrtf(5.0f) - 1.0f) / 4.0f;
static const float G4 = (5.0f - sqrtf(5.0f)) / 20.0f;

float FastNoise::SingleSimplex(unsigned char offset, float x, float y, float z, float w)
{
	float n0, n1, n2, n3, n4;
	float t = (x + y + z + w) * F4;
	int i = FastFloor(x + t);
	int j = FastFloor(y + t);
	int k = FastFloor(z + t);
	int l = FastFloor(w + t);
	t = (i + j + k + l) * G4;
	float X0 = i - t;
	float Y0 = j - t;
	float Z0 = k - t;
	float W0 = l - t;
	float x0 = x - X0;
	float y0 = y - Y0;
	float z0 = z - Z0;
	float w0 = w - W0;

	int c = (x0 > y0) ? 32 : 0;
	c += (x0 > z0) ? 16 : 0;
	c += (y0 > z0) ? 8 : 0;
	c += (x0 > w0) ? 4 : 0;
	c += (y0 > w0) ? 2 : 0;
	c += (z0 > w0) ? 1 : 0;
	c <<= 2;

	int i1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int i2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int i3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int j1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int j2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int j3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int k1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int k2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int k3 = SIMPLEX_4D[c++] >= 1 ? 1 : 0;
	int l1 = SIMPLEX_4D[c] >= 3 ? 1 : 0;
	int l2 = SIMPLEX_4D[c] >= 2 ? 1 : 0;
	int l3 = SIMPLEX_4D[c] >= 1 ? 1 : 0;

	float x1 = x0 - i1 + G4;
	float y1 = y0 - j1 + G4;
	float z1 = z0 - k1 + G4;
	float w1 = w0 - l1 + G4;
	float x2 = x0 - i2 + 2.0f*G4;
	float y2 = y0 - j2 + 2.0f*G4;
	float z2 = z0 - k2 + 2.0f*G4;
	float w2 = w0 - l2 + 2.0f*G4;
	float x3 = x0 - i3 + 3.0f*G4;
	float y3 = y0 - j3 + 3.0f*G4;
	float z3 = z0 - k3 + 3.0f*G4;
	float w3 = w0 - l3 + 3.0f*G4;
	float x4 = x0 - 1.0f + 4.0f*G4;
	float y4 = y0 - 1.0f + 4.0f*G4;
	float z4 = z0 - 1.0f + 4.0f*G4;
	float w4 = w0 - 1.0f + 4.0f*G4;

	t = 0.6f - x0*x0 - y0*y0 - z0*z0 - w0*w0;
	if (t < 0.f) n0 = 0.0f;
	else {
		t *= t;
		n0 = t * t * GradCoord4D(offset, i, j, k, l, x0, y0, z0, w0);
	}
	t = 0.6f - x1*x1 - y1*y1 - z1*z1 - w1*w1;
	if (t < 0.f) n1 = 0.0f;
	else {
		t *= t;
		n1 = t * t * GradCoord4D(offset, i + i1, j + j1, k + k1, l + l1, x1, y1, z1, w1);
	}
	t = 0.6f - x2*x2 - y2*y2 - z2*z2 - w2*w2;
	if (t < 0.f) n2 = 0.0f;
	else {
		t *= t;
		n2 = t * t * GradCoord4D(offset, i + i2, j + j2, k + k2, l + l2, x2, y2, z2, w2);
	}
	t = 0.6f - x3*x3 - y3*y3 - z3*z3 - w3*w3;
	if (t < 0.f) n3 = 0.0f;
	else {
		t *= t;
		n3 = t * t * GradCoord4D(offset, i + i3, j + j3, k + k3, l + l3, x3, y3, z3, w3);
	}
	t = 0.6f - x4*x4 - y4*y4 - z4*z4 - w4*w4;
	if (t < 0.f) n4 = 0.0f;
	else {
		t *= t;
		n4 = t * t * GradCoord4D(offset, i + 1, j + 1, k + 1, l + 1, x4, y4, z4, w4);
	}

	return 27.0f * (n0 + n1 + n2 + n3 + n4);
}

// Cellular Noise
float FastNoise::GetCellular(float x, float y, float z)
{
	x *= m_frequency;
	y *= m_frequency;
	z *= m_frequency;

	switch (m_cellularReturnType)
	{
	case CellValue:
	case NoiseLookup:
	case Distance:
		return SingleCellular(x, y, z);
	default:
		return SingleCellular2Edge(x, y, z);
	}
}

float FastNoise::SingleCellular(float x, float y, float z)
{
	int xr = FastRound(x);
	int yr = FastRound(y);
	int zr = FastRound(z);

	float distance = 999999.f;
	int xc, yc, zc;

	switch (m_cellularDistanceFunction)
	{
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ);

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = (FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

					if (newDistance < distance)
					{
						distance = newDistance;
						xc = xi;
						yc = yi;
						zc = zi;
					}
				}
			}
		}
		break;
	default:
		break;
	}

	unsigned char lutPos;
	switch (m_cellularReturnType)
	{
	case CellValue:
		return ValCoord3D(0, xc, yc, zc);

	case NoiseLookup:
		assert(m_cellularNoiseLookup);

		lutPos = Index3D_256(0, xc, yc, zc);
		return m_cellularNoiseLookup->GetNoise(xc + CELL_3D_X[lutPos], yc + CELL_3D_Y[lutPos], zc + CELL_3D_Z[lutPos]);

	case Distance:
		return distance - 1.0f;
	default:
		return 0.0f;
	}
}

float FastNoise::SingleCellular2Edge(float x, float y, float z)
{
	int xr = FastRound(x);
	int yr = FastRound(y);
	int zr = FastRound(z);

	float distance = 999999.f;
	float distance2 = 999999.f;

	switch (m_cellularDistanceFunction)
	{
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = vecX * vecX + vecY * vecY + vecZ * vecZ;

					distance2 = fmaxf(fminf(distance2, newDistance), distance);
					distance = fminf(distance, newDistance);
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ);

					distance2 = fmaxf(fminf(distance2, newDistance), distance);
					distance = fminf(distance, newDistance);
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				for (int zi = zr - 1; zi <= zr + 1; zi++)
				{
					unsigned char lutPos = Index3D_256(0, xi, yi, zi);

					float vecX = xi - x + CELL_3D_X[lutPos];
					float vecY = yi - y + CELL_3D_Y[lutPos];
					float vecZ = zi - z + CELL_3D_Z[lutPos];

					float newDistance = (FastAbs(vecX) + FastAbs(vecY) + FastAbs(vecZ)) + (vecX * vecX + vecY * vecY + vecZ * vecZ);

					distance2 = fmaxf(fminf(distance2, newDistance), distance);
					distance = fminf(distance, newDistance);
				}
			}
		}
		break;
	default:
		break;
	}

	switch (m_cellularReturnType)
	{
	case Distance2:
		return distance2 - 1.0f;
	case Distance2Add:
		return distance2 + distance - 1.0f;
	case Distance2Sub:
		return distance2 - distance - 1.0f;
	case Distance2Mul:
		return distance2 * distance - 1.0f;
	case Distance2Div:
		return distance / distance2 - 1.0f;
	default:
		return 0.0f;
	}
}

float FastNoise::GetCellular(float x, float y)
{
	x *= m_frequency;
	y *= m_frequency;

	switch (m_cellularReturnType)
	{
	case CellValue:
	case NoiseLookup:
	case Distance:
		return SingleCellular(x, y);
	default:
		return SingleCellular2Edge(x, y);
	}
}

float FastNoise::SingleCellular(float x, float y)
{
	int xr = FastRound(x);
	int yr = FastRound(y);

	float distance = 999999.f;
	int xc, yc;

	switch (m_cellularDistanceFunction)
	{
	default:
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = vecX * vecX + vecY * vecY;

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = (FastAbs(vecX) + FastAbs(vecY));

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = (FastAbs(vecX) + FastAbs(vecY)) + (vecX * vecX + vecY * vecY);

				if (newDistance < distance)
				{
					distance = newDistance;
					xc = xi;
					yc = yi;
				}
			}
		}
		break;
	}

	unsigned char lutPos;
	switch (m_cellularReturnType)
	{
	case CellValue:
		return ValCoord2D(0, xc, yc);

	case NoiseLookup:
		assert(m_cellularNoiseLookup);

		lutPos = Index2D_256(0, xc, yc);
		return m_cellularNoiseLookup->GetNoise(xc + CELL_2D_X[lutPos], yc + CELL_2D_Y[lutPos]);

	case Distance:
		return distance - 1.0f;
	default:
		return 0.0f;
	}
}

float FastNoise::SingleCellular2Edge(float x, float y)
{
	int xr = FastRound(x);
	int yr = FastRound(y);

	float distance = 999999.f;
	float distance2 = 999999.f;

	switch (m_cellularDistanceFunction)
	{
	default:
	case Euclidean:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = vecX * vecX + vecY * vecY;

				distance2 = fmaxf(fminf(distance2, newDistance), distance);
				distance = fminf(distance, newDistance);
			}
		}
		break;
	case Manhattan:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = FastAbs(vecX) + FastAbs(vecY);

				distance2 = fmaxf(fminf(distance2, newDistance), distance);
				distance = fminf(distance, newDistance);
			}
		}
		break;
	case Natural:
		for (int xi = xr - 1; xi <= xr + 1; xi++)
		{
			for (int yi = yr - 1; yi <= yr + 1; yi++)
			{
				unsigned char lutPos = Index2D_256(0, xi, yi);

				float vecX = xi - x + CELL_2D_X[lutPos];
				float vecY = yi - y + CELL_2D_Y[lutPos];

				float newDistance = (FastAbs(vecX) + FastAbs(vecY)) + (vecX * vecX + vecY * vecY);

				distance2 = fmaxf(fminf(distance2, newDistance), distance);
				distance = fminf(distance, newDistance);
			}
		}
		break;
	}

	switch (m_cellularReturnType)
	{
	case Distance2:
		return distance2 - 1.0f;
	case Distance2Add:
		return distance2 + distance - 1.0f;
	case Distance2Sub:
		return distance2 - distance - 1.0f;
	case Distance2Mul:
		return distance2 * distance - 1.0f;
	case Distance2Div:
		return distance / distance2 - 1.0f;
	default:
		return 0.0f;
	}
}

void FastNoise::PositionWarp(float& x, float& y, float& z)
{
	SinglePositionWarp(0, m_positionWarpAmp, m_frequency, x, y, z);
}

void FastNoise::PositionWarpFractal(float& x, float& y, float& z)
{
	float amp = m_positionWarpAmp * m_fractalBounding;
	float freq = m_frequency;
	unsigned int i = 0;

	SinglePositionWarp(m_perm[0], amp, m_frequency, x, y, z);

	while (++i < m_octaves)
	{
		freq *= m_lacunarity;
		amp *= m_gain;
		SinglePositionWarp(m_perm[i], amp, freq, x, y, z);
	}
}

void FastNoise::SinglePositionWarp(unsigned char offset, float warpAmp, float frequency, float& x, float& y, float& z)
{
	float xf = x * frequency;
	float yf = y * frequency;
	float zf = z * frequency;

	int x0 = FastFloor(xf);
	int y0 = FastFloor(yf);
	int z0 = FastFloor(zf);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	float xs, ys, zs;
	switch (m_interp)
	{
	default:
	case Linear:
		xs = xf - (float)x0;
		ys = yf - (float)y0;
		zs = zf - (float)z0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(xf - (float)x0);
		ys = InterpHermiteFunc(yf - (float)y0);
		zs = InterpHermiteFunc(zf - (float)z0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(xf - (float)x0);
		ys = InterpQuinticFunc(yf - (float)y0);
		zs = InterpQuinticFunc(zf - (float)z0);
		break;
	}

	int lutPos0 = Index3D_256(offset, x0, y0, z0);
	int lutPos1 = Index3D_256(offset, x1, y0, z0);

	float lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	float ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	float lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3D_256(offset, x0, y1, z0);
	lutPos1 = Index3D_256(offset, x1, y1, z0);

	float lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	float ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	float lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	float lx0y = Lerp(lx0x, lx1x, ys);
	float ly0y = Lerp(ly0x, ly1x, ys);
	float lz0y = Lerp(lz0x, lz1x, ys);

	lutPos0 = Index3D_256(offset, x0, y0, z1);
	lutPos1 = Index3D_256(offset, x1, y0, z1);

	lx0x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly0x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz0x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	lutPos0 = Index3D_256(offset, x0, y1, z1);
	lutPos1 = Index3D_256(offset, x1, y1, z1);

	lx1x = Lerp(CELL_3D_X[lutPos0], CELL_3D_X[lutPos1], xs);
	ly1x = Lerp(CELL_3D_Y[lutPos0], CELL_3D_Y[lutPos1], xs);
	lz1x = Lerp(CELL_3D_Z[lutPos0], CELL_3D_Z[lutPos1], xs);

	x += Lerp(lx0y, Lerp(lx0x, lx1x, ys), zs) * warpAmp;
	y += Lerp(ly0y, Lerp(ly0x, ly1x, ys), zs) * warpAmp;
	z += Lerp(lz0y, Lerp(lz0x, lz1x, ys), zs) * warpAmp;
}

void FastNoise::PositionWarp(float& x, float& y)
{
	SinglePositionWarp(0, m_positionWarpAmp, m_frequency, x, y);
}

void FastNoise::PositionWarpFractal(float& x, float& y)
{
	float amp = m_positionWarpAmp * m_fractalBounding;
	float freq = m_frequency;
	unsigned int i = 0;

	SinglePositionWarp(m_perm[0], amp, m_frequency, x, y);

	while (++i < m_octaves)
	{
		freq *= m_lacunarity;
		amp *= m_gain;
		SinglePositionWarp(m_perm[i], amp, freq, x, y);
	}
}

void FastNoise::SinglePositionWarp(unsigned char offset, float warpAmp, float frequency, float& x, float& y)
{
	float xf = x * frequency;
	float yf = y * frequency;

	int x0 = FastFloor(xf);
	int y0 = FastFloor(yf);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float xs, ys;
	switch (m_interp)
	{
	default:
	case Linear:
		xs = xf - (float)x0;
		ys = yf - (float)y0;
		break;
	case Hermite:
		xs = InterpHermiteFunc(xf - (float)x0);
		ys = InterpHermiteFunc(yf - (float)y0);
		break;
	case Quintic:
		xs = InterpQuinticFunc(xf - (float)x0);
		ys = InterpQuinticFunc(yf - (float)y0);
		break;
	}

	int lutPos0 = Index2D_256(offset, x0, y0);
	int lutPos1 = Index2D_256(offset, x1, y0);

	float lx0x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	float ly0x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	lutPos0 = Index2D_256(offset, x0, y1);
	lutPos1 = Index2D_256(offset, x1, y1);

	float lx1x = Lerp(CELL_2D_X[lutPos0], CELL_2D_X[lutPos1], xs);
	float ly1x = Lerp(CELL_2D_Y[lutPos0], CELL_2D_Y[lutPos1], xs);

	x += Lerp(lx0x, lx1x, ys) * warpAmp;
	y += Lerp(ly0x, ly1x, ys) * warpAmp;
}
