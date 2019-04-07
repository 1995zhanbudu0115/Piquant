#include "stdafx.h"
#include "DataTypes.h"
#include "../../sdk/ThostFtdcUserApiDataType.h"

vector<BaseData> instruments_data; //ȫ�ֱ��������ص�������ֵ
int GetInstrumentVolumeMultiple(string instrumentID) //��ȡ�ض��ĺ�Լ����
{
	if (instruments_data.empty() != true)
	{//Ҫ��Ϊ��
		for (vector<BaseData>::iterator iter = instruments_data.begin(); iter != instruments_data.end(); iter++)
		{
			string vector_instrumentID = (*iter).code;
			if (vector_instrumentID == instrumentID)
			{
				return (*iter).VolumeMultiple;
			}
		}
		return 0; //�Ҳ����򷵻�0�������������Ҳ���
	}
}




string GetTextById(UserInfo& ui, int field_id)//�����ֶ�ID����UserInfo�û���Ϣ�ṹ���л�ȡ����ֵ
{
	switch (field_id)
	{
	case FIELD_ID_UserInfo::FIELD_UserID:
			return ui.UserID;
			break;
	case FIELD_ID_UserInfo::FIELD_TradingDay:
			return ui.TradingDay;
			break;
	case FIELD_ID_UserInfo::FIELD_LoginTime:
			return ui.LoginTime;
			break;
	case FIELD_ID_UserInfo::FIELD_BrokerID:
			return ui.BrokerID;
			break;
	case FIELD_ID_UserInfo::FIELD_SystemName:
			return ui.SystemName;
			break;
	case FIELD_ID_UserInfo::FIELD_FrontID:
			return to_string(ui.FrontID);
			break;
	case FIELD_ID_UserInfo::FIELD_SessionID:
			return to_string(ui.SessionID);
			break;
	case FIELD_ID_UserInfo::FIELD_MaxOrderRef:
			return ui.MaxOrderRef;
			break;
	case FIELD_ID_UserInfo::FIELD_ExchangeTime:
			return ui.ExchangeTime;
			break;
	default:
			return "--";
			break;
	}
}



string SERVICE_API GetTextById(BaseData& bd, int field_id)
{
	switch (field_id)
	{
	case FIELD_ID_BaseData::FIELD_Code:
		return bd.code;
		break;
	case FIELD_ID_BaseData::FIELD_ExchangeID :
		return bd.ExchangeID;
		break;
	case FIELD_ID_BaseData::FIELD_InstrumentName:
		return bd.InstrumentName;
		break;
	case FIELD_ID_BaseData::FIELD_ExchangeInstID:
		return bd.ExchangeInstID;
		break;
	case FIELD_ID_BaseData::FIELD_ProductID:
		return bd.ProductID;
		break;
	case FIELD_ID_BaseData::FIELD_ProductClass:
		switch (bd.ProductClass)
		{
			case THOST_FTDC_PC_Futures:
				return "�ڻ�";
				break;
			case THOST_FTDC_PC_Options:
				return "�ڻ���Ȩ";
				break;
			case THOST_FTDC_PC_Combination:
				return "���";
				break;
			case THOST_FTDC_PC_Spot:
				return "����";
				break;
			case THOST_FTDC_PC_EFP:
				return "��ת��";
				break;
			case THOST_FTDC_PC_SpotOption:
				return "�ֻ���Ȩ";
				break;
			default:
				return "---";
				break;
		}
	case FIELD_ID_BaseData::FIELD_DeliveryYear:
		return to_string(bd.DeliveryYear);
		break;
	case FIELD_ID_BaseData::FIELD_DeliveryMonth:
		return to_string(bd.DeliveryMonth);
		break;
	case FIELD_ID_BaseData::FIELD_MaxMarketOrderVolume:
		return to_string(bd.MaxMarketOrderVolume);
		break;
	case FIELD_ID_BaseData::FIELD_MinMarketOrderVolume:
		return to_string(bd.MinMarketOrderVolume);
		break;
	case FIELD_ID_BaseData::FIELD_MaxLimitOrderVolume:
		return to_string(bd.MaxLimitOrderVolume);
		break;
	case FIELD_ID_BaseData::FIELD_MinLimitOrderVolume:
		return to_string(bd.MinLimitOrderVolume);
		break;
	case FIELD_ID_BaseData::FIELD_VolumeMultiple:
		return to_string(bd.VolumeMultiple);
		break;
	case FIELD_ID_BaseData::FIELD_PriceTick:
		return to_string(bd.PriceTick);
		break;
	case FIELD_ID_BaseData::FIELD_CreateDate:
		return bd.CreateDate;
		break;
	case FIELD_ID_BaseData::FIELD_OpenDate:
		return bd.OpenDate;
		break;
	case FIELD_ID_BaseData::FIELD_ExpireDate:
		return bd.ExpireDate;
		break;
	case FIELD_ID_BaseData::FIELD_StartDelivDate:
		return bd.StartDelivDate;
		break;
	case FIELD_ID_BaseData::FIELD_EndDelivDate:
		return bd.EndDelivDate;
		break;
	case FIELD_ID_BaseData::FIELD_InstLifePhase:
		switch (bd.InstLifePhase)
		{
			case THOST_FTDC_IP_NotStart:
				return "δ����";
				break;
			case THOST_FTDC_IP_Started:
				return "����";
				break;
			case THOST_FTDC_IP_Pause:
				return "ͣ��";
				break;
			case THOST_FTDC_IP_Expired:
				return "����";
				break;
			default:
				return "---";
				break;
		}
	case FIELD_ID_BaseData::FIELD_IsTrading:
		if (bd.IsTrading == 0) return "��";
		if (bd.IsTrading != 0) return "��";
		break;
	case FIELD_ID_BaseData::FIELD_PositionType:
		switch (bd.PositionType)
		{
			case THOST_FTDC_PT_Net:
				return "���ֲ�";
				break;
			case THOST_FTDC_PT_Gross:
				return "�ۺϳֲ�";
				break;
			default:
				return "---";
				break;
		}
	case FIELD_ID_BaseData::FIELD_PositionDateType:
		switch (bd.PositionDateType)
		{
			case THOST_FTDC_PDT_UseHistory:
				return "ʹ����ʷ�ֲ�";
				break;
			case THOST_FTDC_PDT_NoUseHistory:
				return "��ʹ����ʷ�ֲ�";
				break;
			default:
				return "---";
				break;
		}
	case FIELD_ID_BaseData::FIELD_LongMarginRatio:
		return to_string(bd.LongMarginRatio);
		break;
	case FIELD_ID_BaseData::FIELD_ShortMarginRatio:
		return to_string(bd.ShortMarginRatio);
		break;
	case FIELD_ID_BaseData::FIELD_MaxMarginSideAlgorithm:
		if (bd.MaxMarginSideAlgorithm == 0) return "��";
		if (bd.MaxMarginSideAlgorithm != 0) return "��";
		break;
	case FIELD_ID_BaseData::FIELD_UnderlyingInstrID:
		return bd.UnderlyingInstrID;
		break;
	case FIELD_ID_BaseData::FIELD_StrikePrice:
		return to_string(bd.StrikePrice);
		break;
	case FIELD_ID_BaseData::FIELD_OptionsType:
		switch (bd.OptionsType)
		{
			case THOST_FTDC_CP_CallOptions:
				return "����";
				break;
			case THOST_FTDC_CP_PutOptions:
				return "����";
				break;
			default:
				return "---";
				break;
		}
	case FIELD_ID_BaseData::FIELD_UnderlyingMultiple:
		return to_string(bd.UnderlyingMultiple);
		break;
	case FIELD_ID_BaseData::FIELD_CombinationType:
		switch (bd.CombinationType)
		{
			case THOST_FTDC_COMBT_Future:
				return "�ڻ����";
				break;
			case THOST_FTDC_COMBT_BUL:
				return "��ֱ�۲�BUL";
				break;
			case THOST_FTDC_COMBT_BER:
				return "��ֱ�۲�BER";
				break;
			case THOST_FTDC_COMBT_STD:
				return "��ʽ���";
				break;
			case THOST_FTDC_COMBT_STG:
				return "���ʽ���";
				break;
			case THOST_FTDC_COMBT_PRT:
				return "�������";
				break;
			case THOST_FTDC_COMBT_CLD:
				return "ʱ��۲����";
				break;
			default:
				return "---";
				break;
		}
	default:
		return "--";
		break;
	}
}



string SERVICE_API GetTextById(InvestorPosition& ip, int field_id)
{
	switch (field_id)
	{
	case FIELD_ID_InvestorPosition::FIELD_InstrumentID_Position:
		return ip.InstrumentID;
		break;
	case FIELD_ID_InvestorPosition::FIELD_BrokerID_Position:
		return ip.BrokerID;
		break;
	case FIELD_ID_InvestorPosition::FIELD_InvestorID_Position:
		return ip.InvestorID;
		break;
	case FIELD_ID_InvestorPosition::FIELD_PosiDirection_Position:
	{
		switch (ip.PosiDirection)
		{
		case THOST_FTDC_PD_Net:
			return"��";
			break;
		case THOST_FTDC_PD_Long:
			return"��";
			break;
		case THOST_FTDC_PD_Short:
			return "��";
			break;
		default:
			return"---";
			break;
		}
	}
	case FIELD_ID_InvestorPosition::FIELD_HedgeFlag_Position:
	{
		switch (ip.HedgeFlag)
		{
		case THOST_FTDC_HF_Speculation:
			return "Ͷ��";
			break;
		case THOST_FTDC_HF_Arbitrage:
			return "����";
			break;
		case THOST_FTDC_HF_Hedge:
			return "�ױ�";
			break;
		case THOST_FTDC_HF_MarketMaker:
			return "������";
			break;
		default:
			return "---";
			break;
		}
	}
	case FIELD_ID_InvestorPosition::FIELD_PositionDate_Position:
	{
		switch (ip.PositionDate)
		{
		case THOST_FTDC_PSD_Today:
			return"���ճֲ�";
			break;
		case THOST_FTDC_PSD_History:
			return "��ʷ�ֲ�";
			break;
		default:
			return "---";
			break;
		}
	}
	case FIELD_ID_InvestorPosition::FIELD_YdPosition_Position:
		return to_string(ip.YdPosition);
		break;
	case FIELD_ID_InvestorPosition::FIELD_Position_Position:
		return to_string(ip.Position);
		break;
	case FIELD_ID_InvestorPosition::FIELD_LongFrozen_Position:
		return to_string(ip.LongFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_ShortFrozen_Position:
		return to_string(ip.ShortFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_LongFrozenAmount_Position:
		return to_string(ip.LongFrozenAmount);
		break;
	case FIELD_ID_InvestorPosition::FIELD_ShortFrozenAmount_Position:
		return to_string(ip.ShortFrozenAmount);
		break;
	case FIELD_ID_InvestorPosition::FIELD_OpenVolume_Position:
		return to_string(ip.OpenVolume);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CloseVolume_Position:
		return to_string(ip.CloseVolume);
		break;
	case FIELD_ID_InvestorPosition::FIELD_OpenAmount_Position:
		return to_string(ip.OpenAmount);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CloseAmount_Position:
		return to_string(ip.CloseAmount);
		break;
	case FIELD_ID_InvestorPosition::FIELD_PositionCost_Position:
		return to_string(ip.PositionCost);
		break;
	case FIELD_ID_InvestorPosition::FIELD_PreMargin_Position:
		return to_string(ip.PreMargin);
		break;
	case FIELD_ID_InvestorPosition::FIELD_UseMargin_Position:
	{
		char usemargin[100];
		sprintf(usemargin, "%0.2f", ip.UseMargin); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return usemargin;
		break;
	}
	case FIELD_ID_InvestorPosition::FIELD_FrozenMargin_Position:
		return to_string(ip.FrozenMargin);
		break;
	case FIELD_ID_InvestorPosition::FIELD_FrozenCash_Position:
		return to_string(ip.FrozenCash);
		break;
	case FIELD_ID_InvestorPosition::FIELD_FrozenCommission_Position:
		return to_string(ip.FrozenCommission);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CashIn_Position:
		return to_string(ip.CashIn);
		break;
	case FIELD_ID_InvestorPosition::FIELD_Commission_Position:
		return to_string(ip.Commission);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CloseProfit_Position:
		return to_string(ip.CloseProfit);
		break;
	case FIELD_ID_InvestorPosition::FIELD_PositionProfit_Position:
	{
		char positionprofit[100];
		sprintf(positionprofit, "%0.2f", ip.PositionProfit); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return positionprofit;
		break;
	}
	case FIELD_ID_InvestorPosition::FIELD_PreSettlementPrice_Position:
		return to_string(ip.PreSettlementPrice);
		break;
	case FIELD_ID_InvestorPosition::FIELD_SettlementPrice_Position:
	{
		char settlementprice[100];
		sprintf(settlementprice, "%0.2f", ip.SettlementPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return settlementprice;
		break;
	}
	case FIELD_ID_InvestorPosition::FIELD_TradingDay_Position:
		return ip.TradingDay;
		break;
	case FIELD_ID_InvestorPosition::FIELD_SettlementID_Position:
		return to_string(ip.SettlementID);
		break;
	case FIELD_ID_InvestorPosition::FIELD_OpenCost_Position:
		return to_string(ip.OpenCost);
		break;
	case FIELD_ID_InvestorPosition::FIELD_ExchangeMargin_Position:
		return to_string(ip.ExchangeMargin);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CombPosition_Position:
		return to_string(ip.CombPosition);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CombLongFrozen_Position:
		return to_string(ip.CombLongFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CombShortFrozen_Position:
		return to_string(ip.CombShortFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CloseProfitByDate_Position:
		return to_string(ip.CloseProfitByDate);
		break;
	case FIELD_ID_InvestorPosition::FIELD_CloseProfitByTrade_Position:
		return to_string(ip.CloseProfitByTrade);
		break;
	case FIELD_ID_InvestorPosition::FIELD_TodayPosition_Position:
		return to_string(ip.TodayPosition);
		break;
	case FIELD_ID_InvestorPosition::FIELD_MarginRateByMoney_Position:
		return to_string(ip.MarginRateByMoney);
		break;
	case FIELD_ID_InvestorPosition::FIELD_MarginRateByVolume_Position:
		return to_string(ip.MarginRateByVolume);
		break;
	case FIELD_ID_InvestorPosition::FIELD_StrikeFrozen_Position:
		return to_string(ip.StrikeFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_StrikeFrozenAmount_Position:
		return to_string(ip.StrikeFrozenAmount);
		break;
	case FIELD_ID_InvestorPosition::FIELD_AbandonFrozen_Position:
		return to_string(ip.AbandonFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_ExchangeID_Position:
		return ip.ExchangeID;
		break;
	case FIELD_ID_InvestorPosition::FIELD_YdStrikeFrozen_Position:
		return to_string(ip.YdStrikeFrozen);
		break;
	case FIELD_ID_InvestorPosition::FIELD_InvestUnitID_Position:
		return ip.InvestUnitID;
		break;
	case FIELD_ID_InvestorPosition::FIELD_FloatProfit_Position:
	{
		if (GetInstrumentVolumeMultiple(ip.InstrumentID) != 0)
		{
			double FloatProfit = (ip.SettlementPrice - ip.OpenCost / GetInstrumentVolumeMultiple(ip.InstrumentID)/ip.Position) * ip.Position *GetInstrumentVolumeMultiple(ip.InstrumentID);
			char floatprofit[100];
			if (ip.PosiDirection == THOST_FTDC_PD_Long) //����ͷ
			{
				sprintf(floatprofit, "%0.2f", FloatProfit); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
				return floatprofit;
			}
			if (ip.PosiDirection == THOST_FTDC_PD_Short) //����ͷ
			{
				sprintf(floatprofit, "%0.2f", -1 * FloatProfit); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
				return floatprofit;
			}
			break;
		}
		else
		{
			return "---";
			break;
		}
		/*
		��ͷ����ӯ��=�����ν����-���ֳɱ�/��Լ����/������*����*��Լ����
		��ͷ����ӯ��=-1*�����ν����-���ֳɱ�/��Լ����/������*����*��Լ����
		*/
	}
	case FIELD_ID_InvestorPosition::FIELD_AvgPrice_Position:
	{
		if (GetInstrumentVolumeMultiple(ip.InstrumentID) != 0)
		{
			char avgprice[100];
			sprintf(avgprice, "%0.2f", ip.OpenCost / GetInstrumentVolumeMultiple(ip.InstrumentID)/ip.Position); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
			return avgprice;
			break;
		}
		else
		{
			return "---";
			break;
		}
		/*
		���־���=���ֳɱ�/��Լ����/����
		*/
	}
	default:
		return "---";
		break;
	}
}


string SERVICE_API GetTextById(Order& o, int field_id)
{
	switch (field_id)
	{
	case FIELD_BrokerID_Order:
		return o.BrokerID;
		break;
	case FIELD_InvestorID_Order:
		return o.InvestorID;
		break;
	case FIELD_InstrumentID_Order:
		return o.InstrumentID;
		break;
	case FIELD_OrderRef_Order:
		return o.OrderRef;
		break;
	case FIELD_UserID_Order:
		return o.UserID;
		break;
	case FIELD_OrderPriceType_Order:
	{
		if (o.OrderPriceType == THOST_FTDC_OPT_AnyPrice)	return"�м�";
		else if (o.OrderPriceType == THOST_FTDC_OPT_LimitPrice) return "�޼�";
		else return"---";
		break;
	}
	case FIELD_Direction_Order:
	{
		if (o.Direction == THOST_FTDC_D_Buy) return "��";
		else if (o.Direction == THOST_FTDC_D_Sell) return "��";
		else return "---";
		break;
	}
	case FIELD_CombOffsetFlag_Order:
	{
		if (o.CombOffsetFlag[0] == '0')	return "����";
		else if (o.CombOffsetFlag[0] == '1')	return "ƽ��";
		else if (o.CombOffsetFlag[0] == '3')	return "ƽ��";
		else return "---";
		break;
	}
	case FIELD_CombHedgeFlag_Order:
		return (char*)o.CombHedgeFlag[0];
		break;
	case FIELD_LimitPrice_Order:
	{
		char LimitPrice[100];
		sprintf(LimitPrice, "%0.2f", o.LimitPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return LimitPrice;
		break;
	}
	case FIELD_VolumeTotalOriginal_Order:
		return to_string(o.VolumeTotalOriginal);
		break;
	case FIELD_TimeCondition_Order:
		return (char*)o.TimeCondition;
		break;
	case FIELD_GTDDate_Order:
		return o.GTDDate;
		break;
	case FIELD_VolumeCondition_Order:
	{
		if (o.VolumeCondition == THOST_FTDC_VC_AV) return"��������";
		else if (o.VolumeCondition == THOST_FTDC_VC_MV) return"��С����";
		else if (o.VolumeCondition == THOST_FTDC_VC_CV) return"ȫ������";
		else return "---";
		break;
	}
	case FIELD_MinVolume_Order:
		return to_string(o.MinVolume);
		break;
	case FIELD_ContingentCondition_Order:
	{
		if (o.ContingentCondition == THOST_FTDC_CC_Immediately)  return "����";
		else if (o.ContingentCondition == THOST_FTDC_CC_Touch) return "ֹ��";
		else if (o.ContingentCondition == THOST_FTDC_CC_TouchProfit) return "ֹӯ";
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice) return "����ͻ��";
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice) return "����ͻ��";
		else return "---";
		break;
	}
	case FIELD_StopPrice_Order:
	{
		char StopPrice[100];
		sprintf(StopPrice, "%0.2f", o.StopPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return StopPrice;
		break;
	}
	case FIELD_ForceCloseReason_Order:
		return (char*)o.ForceCloseReason;
		break;
	case FIELD_IsAutoSuspend_Order:
		return (char*)o.IsAutoSuspend;
		break;
	case FIELD_BusinessUnit_Order:
		return o.BusinessUnit;
		break;
	case FIELD_RequestID_Order:
		return to_string(o.RequestID);
		break;
	case FIELD_OrderLocalID_Order:
		return o.OrderLocalID;
		break;
	case FIELD_ExchangeID_Order:
		return o.ExchangeID;
		break;
	case FIELD_ParticipantID_Order:
		return o.ParticipantID;
		break;
	case FIELD_ClientID_Order:
		return o.ClientID;
		break;
	case FIELD_ExchangeInstID_Order:
		return o.ExchangeInstID;
		break;
	case FIELD_TraderID_Order:
		return o.TraderID;
		break;
	case FIELD_InstallID_Order:
		return to_string(o.InstallID);
		break;
	case FIELD_OrderSubmitStatus_Order:
	{
		if (o.OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted) return "�Ѿ��ύ";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_CancelSubmitted) return "�����Ѿ��ύ";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_ModifySubmitted) return "�޸��Ѿ��ύ";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_Accepted) return "�Ѿ�����";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_InsertRejected) return "�����Ѿ����ܾ�";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_CancelRejected) return "�����Ѿ����ܾ�";
		else if (o.OrderSubmitStatus == THOST_FTDC_OSS_ModifyRejected) return "�ĵ��Ѿ����ܾ�";
		else return"---";
		break;
	}
	case FIELD_NotifySequence_Order:
		return to_string(o.NotifySequence);
		break;
	case FIELD_TradingDay_Order:
		return o.TradingDay;
		break;
	case FIELD_SettlementID_Order:
		return to_string(o.SettlementID);
		break;
	case FIELD_OrderSysID_Order:
		return o.OrderSysID;
		break;
	case FIELD_OrderSource_Order:
		return (char*)o.OrderSource;
		break;
	case FIELD_OrderStatus_Order:
	{
		if (o.OrderStatus == THOST_FTDC_OST_AllTraded) return "ȫ���ɽ�";
		else if (o.OrderStatus == THOST_FTDC_OST_PartTradedQueueing) return "���ֳɽ����ڶ�����";
		else if (o.OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing) return "���ֳɽ����ڶ�����";
		else if (o.OrderStatus == THOST_FTDC_OST_NoTradeQueueing) return "δ�ɽ����ڶ�����";
		else if (o.OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing) return "δ�ɽ����ڶ�����";
		else if (o.OrderStatus == THOST_FTDC_OST_Canceled) return "����";
		else if (o.OrderStatus == THOST_FTDC_OST_Unknown) return "δ֪";
		else if (o.OrderStatus == THOST_FTDC_OST_NotTouched) return "��δ����";
		else if (o.OrderStatus == THOST_FTDC_OST_Touched) return "�Ѵ���";
		else return "---";
		break;
	}
	case FIELD_OrderType_Order:
	{
		if (o.OrderType == THOST_FTDC_ORDT_Normal) return "����";
		else if (o.OrderType == THOST_FTDC_ORDT_DeriveFromQuote) return "��������";
		else if (o.OrderType == THOST_FTDC_ORDT_DeriveFromCombination) return "�������";
		else if (o.OrderType == THOST_FTDC_ORDT_Combination) return "��ϱ���";
		else if (o.OrderType == THOST_FTDC_ORDT_ConditionalOrder) return "������";
		else if (o.OrderType == THOST_FTDC_ORDT_Swap) return "������";
		else return "---";
		break;
	}
	case FIELD_VolumeTraded_Order:
		return to_string(o.VolumeTraded);
		break;
	case FIELD_VolumeTotal_Order:
		return to_string(o.VolumeTotal);
		break;
	case FIELD_InsertDate_Order:
		return o.InsertDate;
		break;
	case FIELD_InsertTime_Order:
		return o.InsertTime;
		break;
	case FIELD_ActiveTime_Order:
		return o.ActiveTime;
		break;
	case FIELD_SuspendTime_Order:
		return o.SuspendTime;
		break;
	case FIELD_UpdateTime_Order:
		return o.UpdateTime;
		break;
	case FIELD_CancelTime_Order:
		return o.CancelTime;
		break;
	case FIELD_ActiveTraderID_Order:
		return o.ActiveTraderID;
		break;
	case FIELD_ClearingPartID_Order:
		return o.ClearingPartID;
		break;
	case FIELD_SequenceNo_Order:
		return to_string(o.SequenceNo);
		break;
	case FIELD_FrontID_Order:
		return to_string(o.FrontID);
		break;
	case FIELD_SessionID_Order:
		return to_string(o.SessionID);
		break;
	case FIELD_UserProductInfo_Order:
		return o.UserProductInfo;
		break;
	case FIELD_StatusMsg_Order:
		return o.StatusMsg;
		break;
	case FIELD_UserForceClose_Order:
		return to_string(o.UserForceClose);
		break;
	case FIELD_ActiveUserID_Order:
		return o.ActiveUserID;
		break;
	case FIELD_BrokerOrderSeq_Order:
		return to_string(o.BrokerOrderSeq);
		break;
	case FIELD_RelativeOrderSysID_Order:
		return o.RelativeOrderSysID;
		break;
	case FIELD_ZCETotalTradedVolume_Order:
		return to_string(o.ZCETotalTradedVolume);
		break;
	case FIELD_IsSwapOrder_Order:
		return to_string(o.IsSwapOrder);
		break;
	case FIELD_BranchID_Order:
		return o.BranchID;
		break;
	case FIELD_InvestUnitID_Order:
		return o.InvestUnitID;
		break;
	case FIELD_AccountID_Order:
		return o.AccountID;
		break;
	case FIELD_CurrencyID_Order:
		return o.CurrencyID;
		break;
	case FIELD_IPAddress_Order:
		return o.IPAddress;
		break;
	case FIELD_MacAddress_Order:
		return o.MacAddress;
		break;
	case FIELD_OrderLocalType_Order:
	{
		if (o.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice && o.Direction == THOST_FTDC_D_Buy && o.CombOffsetFlag[0] == '0')
		{
			return "BuyStop";
			break;
		}
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice && o.Direction == THOST_FTDC_D_Sell  && o.CombOffsetFlag[0] == '0')
		{
			return "SellLimit";
			break;
		}
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice && o.Direction == THOST_FTDC_D_Buy && o.CombOffsetFlag[0] == '0')
		{
			return "BuyLimit";
			break;
		}
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice && o.Direction == THOST_FTDC_D_Sell  && o.CombOffsetFlag[0] == '0')
		{
			return "SellStop";
			break;
		}
		else if (o.ContingentCondition == THOST_FTDC_CC_LastPriceLesserEqualStopPrice || o.ContingentCondition == THOST_FTDC_CC_LastPriceGreaterEqualStopPrice)
		{
			if (o.CombOffsetFlag[0] == '1' || o.CombOffsetFlag[0] == '3' )//ƽ�ּ�ƽ��
			{//��ʱ�򻯣�������ֹӯ��ֹ��
				return "ProfitLoss";
				break;
			}
		}
		else
		{
			return "---";
		}
	}
	default:
		break;
	}
}


string SERVICE_API GetTextById(Trade& t, int field_id)
{
	switch (field_id)
	{
	case FIELD_BrokerID_Trade:
		return t.BrokerID;
		break;
	case FIELD_InvestorID_Trade:
		return t.InvestorID;
		break;
	case FIELD_InstrumentID_Trade:
		return t.InstrumentID;
		break;
	case FIELD_OrderRef_Trade:
		return t.OrderRef;
		break;
	case FIELD_UserID_Trade:
		return t.UserID;
		break;
	case FIELD_ExchangeID_Trade:
		return t.ExchangeID;
		break;
	case FIELD_TradeID_Trade:
		return t.TradeID;
		break;
	case FIELD_Direction_Trade:
	{
		switch (t.Direction)
		{
			case THOST_FTDC_D_Buy:
				return "��";
				break;
			case THOST_FTDC_D_Sell:
				return "��";
				break;
			default:
				return "---";
				break;
		}
	}
	case FIELD_OrderSysID_Trade:
		return t.OrderSysID;
		break;
	case FIELD_ParticipantID_Trade:
		return t.ParticipantID;
		break;
	case FIELD_ClientID_Trade:
		return t.ClientID;
		break;
	case FIELD_TradingRole_Trade:
	{
		switch (t.TradingRole)
		{
			case THOST_FTDC_ER_Broker:
				return "����";
				break;
			case THOST_FTDC_ER_Host:
				return "��Ӫ";
				break;
			case THOST_FTDC_ER_Maker:
				return "������";
				break;
			default:
				return "---";
				break;
		}
	}
	case FIELD_ExchangeInstID_Trade:
		return t.ExchangeInstID;
		break;
	case FIELD_OffsetFlag_Trade:
	{
		switch (t.OffsetFlag)
		{
			case THOST_FTDC_OF_Open:
				return "����";
				break;
			case THOST_FTDC_OF_Close:
				return "ƽ��";
				break;
			case THOST_FTDC_OF_ForceClose:
				return "ǿƽ";
				break;
			case THOST_FTDC_OF_CloseToday:
				return "ƽ��";
				break;
			case THOST_FTDC_OF_CloseYesterday:
				return "ƽ��";
				break;
			case THOST_FTDC_OF_ForceOff:
				return "ǿ��";
				break;
			case THOST_FTDC_OF_LocalForceClose:
				return "����ǿƽ";
				break;
			default:
				return "---";
				break;
		}
	}
	case FIELD_HedgeFlag_Trade:
	{
		switch (t.HedgeFlag)
		{
			case THOST_FTDC_HF_Speculation:
				return "Ͷ��";
				break;
			case THOST_FTDC_HF_Arbitrage:
				return "����";
				break;
			case THOST_FTDC_HF_Hedge:
				return "�ױ�";
				break;
			case THOST_FTDC_HF_MarketMaker:
				return "������";
				break;
			default:
				return "---";
				break;
		}
	}
		break;
	case FIELD_Price_Trade:
	{
		char Price[100];
		sprintf(Price, "%0.2f", t.Price); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return Price;
		break;
	}	
	case FIELD_Volume_Trade:
		return to_string(t.Volume);
		break;
	case FIELD_TradeDate_Trade:
		return t.TradeDate;
		break;
	case FIELD_TradeTime_Trade:
		return t.TradeTime;
		break;
	case FIELD_TradeType_Trade:
		return (char*)t.TradeType;
		break;
	case FIELD_PriceSource_Trade:
	{
		switch (t.PriceSource)
		{
		case THOST_FTDC_PSRC_LastPrice:
			return "ǰ�ɽ���";
			break;
		case THOST_FTDC_PSRC_Buy:
			return "��ί�м�";
			break;
		case THOST_FTDC_PSRC_Sell:
			return "��ί�м�";
			break;
		default:
			return "---";
			break;
		}
	}
		break;
	case FIELD_TraderID_Trade:
		return t.TraderID;
		break;
	case FIELD_OrderLocalID_Trade:
		return t.OrderLocalID;
		break;
	case FIELD_ClearingPartID_Trade:
		return t.ClearingPartID;
		break;
	case FIELD_BusinessUnit_Trade:
		return t.BusinessUnit;
		break;
	case FIELD_SequenceNo_Trade:
		return to_string(t.SequenceNo);
		break;
	case FIELD_TradingDay_Trade:
		return t.TradingDay;
		break;
	case FIELD_SettlementID_Trade:
		return to_string(t.SettlementID);
		break;
	case FIELD_BrokerOrderSeq_Trade:
		return to_string(t.BrokerOrderSeq);
		break;
	case FIELD_TradeSource_Trade:
		return (char*)t.TradeSource;
		break;
	case FIELD_InvestUnitID_Trade:
		return t.InvestUnitID;
		break;
	default:
		return "---";
		break;
	}
}



string SERVICE_API GetTextById(InvestorPositionDetail& ipd, int field_id)
{
	switch (field_id)
	{
	case FIELD_InstrumentID_PositionDetail:
		return ipd.InstrumentID;
		break;
	case FIELD_BrokerID_PositionDetail:
		return ipd.BrokerID;
		break;
	case FIELD_InvestorID_PositionDetail:
		return ipd.InvestorID;
		break;
	case FIELD_Direction_PositionDetail:
	{
		switch (ipd.Direction)
		{
		case THOST_FTDC_D_Buy:
			return "��";
			break;
		case THOST_FTDC_D_Sell:
			return "��";
			break;
		default:
			return "---";
			break;
		}
	}
	case FIELD_HedgeFlag_PositionDetail:
	{
		switch (ipd.HedgeFlag)
		{
		case THOST_FTDC_HF_Speculation:
			return "Ͷ��";
			break;
		case THOST_FTDC_HF_Arbitrage:
			return "����";
			break;
		case THOST_FTDC_HF_Hedge:
			return "�ױ�";
			break;
		case THOST_FTDC_HF_MarketMaker:
			return "������";
			break;
		default:
			return "---";
			break;
		}
	}
	case FIELD_OpenDate_PositionDetail:
		return ipd.OpenDate;
		break;
	case FIELD_TradeID_PositionDetail:
		return ipd.TradeID;
		break;
	case FIELD_Volume_PositionDetail:
		return to_string(ipd.Volume);
		break;
	case FIELD_OpenPrice_PositionDetail:
	{
		char Price[100];
		sprintf(Price, "%0.2f", ipd.OpenPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return Price;
		break;
	}
	case FIELD_TradingDay_PositionDetail:
		return ipd.TradingDay;
		break;
	case FIELD_SettlementID_PositionDetail:
		return to_string(ipd.SettlementID);
		break;
	case FIELD_TradeType_PositionDetail:
	{
		switch (ipd.TradeType)
		{
		case THOST_FTDC_TRDT_SplitCombination:
			return "��ϳֲֲ��Ϊ��һ�ֲ�";
			break;
		case THOST_FTDC_TRDT_Common:
			return "��ͨ�ɽ�";
			break;
		case THOST_FTDC_TRDT_OptionsExecution:
			return "��Ȩִ��";
			break;
		case THOST_FTDC_TRDT_OTC:
			return "OTC�ɽ�";
			break;
		case THOST_FTDC_TRDT_EFPDerived:
			return "��ת�������ɽ�";
			break;
		case THOST_FTDC_TRDT_CombinationDerived:
			return "��������ɽ�";
			break;
		default:
			return "---";
			break;
		}
	}
	case FIELD_CombInstrumentID_PositionDetail:
		return ipd.CombInstrumentID;
		break;
	case FIELD_ExchangeID_PositionDetail:
		return ipd.ExchangeID;
		break;
	case FIELD_CloseProfitByDate_PositionDetail:
	{
		char CloseProfitByDate[100];
		sprintf(CloseProfitByDate, "%0.2f", ipd.CloseProfitByDate); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return CloseProfitByDate;
		break;
	}
	case FIELD_CloseProfitByTrade_PositionDetail:
	{
		char CloseProfitByTrade[100];
		sprintf(CloseProfitByTrade, "%0.2f", ipd.CloseProfitByTrade); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return CloseProfitByTrade;
		break;
	}
	case FIELD_PositionProfitByDate_PositionDetail:
	{
		char PositionProfitByDate[100];
		sprintf(PositionProfitByDate, "%0.2f", ipd.PositionProfitByDate); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return PositionProfitByDate;
		break;
	}
	case FIELD_PositionProfitByTrade_PositionDetail:
	{
		char PositionProfitByTrade_value[100];
		sprintf(PositionProfitByTrade_value, "%0.2f", ipd.PositionProfitByTrade); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return PositionProfitByTrade_value;
		break;
	}
	/*
	���ն���ƽ��ӯ��=��ƽ�ּۣ������ۣ�����������Լ��λ�����Կ�����ʷ��Ϊ��
	��ʶԳ�ƽ��ӯ��=��ƽ�ּ�-���ּۣ�*����*��Լ��λ��			
	���ն��гֲ�ӯ���������ս���ۣ������ۣ�����������Լ��λ�����Կ�����ʷ��Ϊ��
	��ʶԳ�ֲ�ӯ��=�����ս����-���ּۣ�*����*��Լ��λ�� 
	*/
	case FIELD_Margin_PositionDetail:
	{
		char Margin[100];
		sprintf(Margin, "%0.2f", ipd.Margin); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return Margin;
		break;
	}
	case FIELD_ExchMargin_PositionDetail:
	{
		char ExchMargin[100];
		sprintf(ExchMargin, "%0.2f", ipd.ExchMargin); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return ExchMargin;
		break;
	}
	case FIELD_MarginRateByMoney_PositionDetail:
	{
		char MarginRateByMoney[100];
		sprintf(MarginRateByMoney, "%0.2f", ipd.MarginRateByMoney); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return MarginRateByMoney;
		break;
	}
	case FIELD_MarginRateByVolume_PositionDetail:
	{
		char MarginRateByVolume[100];
		sprintf(MarginRateByVolume, "%0.2f", ipd.MarginRateByVolume); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return MarginRateByVolume;
		break;
	}
	case FIELD_LastSettlementPrice_PositionDetail:
	{
		char LastSettlementPrice[100];
		sprintf(LastSettlementPrice, "%0.2f", ipd.LastSettlementPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return LastSettlementPrice;
		break;
	}
	case FIELD_SettlementPrice_PositionDetail:
	{
		char SettlementPrice[100];
		sprintf(SettlementPrice, "%0.2f", ipd.SettlementPrice); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return SettlementPrice;
		break;
	}
	case FIELD_CloseVolume_PositionDetail:
	{
		return to_string(ipd.CloseVolume);
		break;
	}
	case FIELD_CloseAmount_PositionDetail:
	{
		char CloseAmount[100];
		sprintf(CloseAmount, "%0.2f", ipd.CloseAmount); //��ȡΪ��λ���Ⱥ�ת��Ϊ�ַ�������
		return CloseAmount;
		break;
	}
	case FIELD_InvestUnitID_PositionDetail:
	{
		return ipd.InvestUnitID;
		break;
	}
	default:
		return "---";
		break;
	}
}


