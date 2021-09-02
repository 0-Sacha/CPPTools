#pragma once

#include "CPPTools/Formatter/Core/Detail.h"
#include "BaseFormat/UnFormatType.h"
#include "../FormatContext/BasicFormatContext.h"

namespace CPPTools::Fmt::Detail {

	template <typename... Types>
	struct UnFormatContextArgsTuple;

    template <>
    struct UnFormatContextArgsTuple<> {
    public:
        UnFormatContextArgsTuple() = default;

	public:
        static inline constexpr std::size_t Size() { return 0; }

    public:
        template <typename UnFormatContext>
        inline void ReadTypeFromIdx(UnFormatContext &context, FormatIdx idx) {}

        template <typename UnFormatContext>
        inline void GetParameterDataFromIdx(UnFormatContext &context, FormatIdx idx) {}

        template <typename UnFormatContext>
        inline void GetParameterData(UnFormatContext &context, FormatIdx idx) { context.GetFormatData() = UnFormatContext::FormatDataType(); }

        template <typename UnFormatContext>
        inline void GetNamedArgsIdx(UnFormatContext& context, FormatIdx& idx, FormatIdx currentIdx) { idx = FormatIdxNotFound; }

        template <class ValueType>
        inline void GetFormatValueAt(ValueType& value, FormatIdx idx) {}
    };





    template <typename Type, typename... Rest>
    struct UnFormatContextArgsTuple<Type, Rest...> : UnFormatContextArgsTuple<Rest...>
    {
    private:
        using TypeWithoutRef = std::remove_reference_t<Type>;

    public:
        UnFormatContextArgsTuple(TypeWithoutRef &t, Rest &&...rest)     : UnFormatContextArgsTuple<Rest...>(std::forward<Rest>(rest)...), m_Value(t) {}
        UnFormatContextArgsTuple(TypeWithoutRef &&t, Rest &&...rest)    : UnFormatContextArgsTuple<Rest...>(std::forward<Rest>(rest)...), m_Value(t) {}

	public:
		static inline constexpr std::size_t Size() { return sizeof...(Rest) + 1; }

    public:
        TypeWithoutRef &m_Value;

    public:
        /////---------- FormatTypeFromIdx ----------/////
        template <typename UnFormatContext>
        inline void ReadTypeFromIdx(UnFormatContext &context, FormatIdx idx) {
		    if (idx == 0)       context.ReadType(m_Value);
		    else if(idx > 0)	UnFormatContextArgsTuple<Rest...>::ReadTypeFromIdx(context, idx - 1);
        }

        /////---------- GetParameterDataFromIdx ----------/////
        template <typename UnFormatContext, class KType = TypeWithoutRef>
        inline auto GetParameterDataFromIdx(UnFormatContext &context, FormatIdx idx) -> std::enable_if_t<std::is_same_v<Detail::GetBaseType<KType>, typename UnFormatContext::FormatDataType>> {
            if (idx == 0)       context.GetFormatData() = m_Value;
            else if (idx > 0)   UnFormatContextArgsTuple<Rest...>::GetParameterDataFromIdx(context, idx - 1);
        }

        template <typename UnFormatContext, class KType = TypeWithoutRef>
        inline auto GetParameterDataFromIdx(UnFormatContext &context, FormatIdx idx) -> std::enable_if_t<!std::is_same_v<Detail::GetBaseType<KType>, typename UnFormatContext::FormatDataType>> {
            if (idx > 0)        UnFormatContextArgsTuple<Rest...>::GetParameterDataFromIdx(context, idx - 1);
        }


        /////---------- GetNamedArgsIdx ----------/////
        template<typename UnFormatContext, class KType = TypeWithoutRef>
        inline auto GetNamedArgsIdx(UnFormatContext& context, FormatIdx& idx, FormatIdx currentIdx) -> std::enable_if_t<!Detail::IsANamedArgs<Detail::GetBaseType<KType>>::value>{
            UnFormatContextArgsTuple<Rest...>::GetNamedArgsIdx(context, idx, currentIdx + 1);
        }

        template<typename UnFormatContext, class KType = TypeWithoutRef>
        inline auto GetNamedArgsIdx(UnFormatContext& context, FormatIdx& idx, FormatIdx currentIdx) -> std::enable_if_t<Detail::IsANamedArgs<Detail::GetBaseType<KType>>::value>{
            if (context.FormatNextIsANamedArgs(m_Value.GetName()))    idx = currentIdx;
            else                                                    UnFormatContextArgsTuple<Rest...>::GetNamedArgsIdx(context, idx, currentIdx + 1);
        }

        /////---------- GetFormatValueAt ----------/////
        template <class ValueType, class KType = TypeWithoutRef>
        inline auto GetFormatValueAt(ValueType& value, FormatIdx idx) -> std::enable_if_t<!std::is_convertible_v<KType, ValueType>> {
		    if (idx > 0)	    UnFormatContextArgsTuple<Rest...>::GetFormatValueAt(value, idx - 1);
        }

        template <class ValueType, class KType = TypeWithoutRef>
        inline auto GetFormatValueAt(ValueType& value, FormatIdx idx) -> std::enable_if_t<std::is_convertible_v<KType, ValueType>> {
		    if (idx == 0)		value = static_cast<ValueType>(m_Value);
		    else if(idx > 0)    UnFormatContextArgsTuple<Rest...>::GetFormatValueAt(value, idx - 1);
        }
    };
}